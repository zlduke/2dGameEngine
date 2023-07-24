#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent
{
    std::string assetId;
    int width;
    int height;
    int zIndex; // large --> foreground
    SDL_Rect srcRect;
    // need default value to pass the compile.
    SpriteComponent(std::string assetId = "",
                    int width = 0,
                    int height = 0,
                    int zIndex = 0,
                    int srcRectX = 0,
                    int srcRectY = 0)
    {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRect = {srcRectX, srcRectY, width, height};
    };
};

#endif
