#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"

Game::Game()
{
    isRunning = false;
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
    playerPosition = glm::vec2(10.0, 20.0);
    playerVelocity = glm::vec2(10.0, 0.0);
    millisecsPreviousFrame = SDL_GetTicks();
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
    millisecsPreviousFrame = SDL_GetTicks();

    // update player locations etc.
    playerPosition.x += playerVelocity.x * deltaTime;
    playerPosition.y += playerVelocity.y * deltaTime;
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

    // Load a PNG --> surface --> texture --> render
    SDL_Surface *surface = IMG_Load("./assets/images/tank-panther-right.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dstRect = {
        static_cast<int>(playerPosition.x),
        static_cast<int>(playerPosition.y),
        50,
        50};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_RenderPresent(renderer);
    // double-buffer: alternate front and back buffers
}
void Game::Destroy()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
