// header guard
#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
    //
    bool isRunning;
    int millisecsPreviousFrame;
    // SDL is written in C, as "opaque object"
    // we would need some wrapper such as std::unique_ptr<SDLxx, DestroySDLxx> to make it work.
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Registry> registry;

public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
    void Setup();

    int windowWidth;
    int windowHeight;
};

#endif
