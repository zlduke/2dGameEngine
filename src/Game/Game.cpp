#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

Game::Game()
{
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    Logger::Log("Game constructor is called!");
}

Game::~Game()
{
    Logger::Log("Game destructor is called!");
}
void Game::Initialize()
{
    // examine hardware requirements (e.g., headless linux server)
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error in initializing SDL");
        return;
    }
    // try to open an window
    const std::string window_name = "my game";
    SDL_DisplayMode displayMode;
    // fake a full screen by getting the screen size
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    window = SDL_CreateWindow(NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight,
                              SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        Logger::Err("Error in creating SDL window");
        return;
    }
    // try to attach a renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        Logger::Err("Error in creating SDL renderer");
        return;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::Setup()
{
    // Add systems
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();

    // Adding assets to assetStore
    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

    // Add entity
    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(3.0, 3.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(40, 0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0, 50.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
}

void Game::Update()
{
    // Frame limiter
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
    {

        SDL_Delay(timeToWait);
    }

    // the actual elapsed time since the last frame
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    // Store the previous frame time
    millisecsPreviousFrame = SDL_GetTicks();

    // Ask all the systems to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);

    // In the end: Update the registry to process the queued entites to be added
    registry->Update();
}

void Game::Run()
{
    Setup();
    while (isRunning)
    {
        ProcessInput();
        Update(); // <-- delay implementation
        Render();
    }
}
void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    // process and remove event(s) in the queue
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
                break;
            }
        }
    }
}
void Game::Render()
{
    // set up canvas
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer); // this is actually filling colors, the naming is misleading

    // invoke all systems that need to render
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

    SDL_RenderPresent(renderer);
    // double-buffer: alternate front and back buffers
}
void Game::Destroy()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
