#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent
{
    int width;
    int height;
    // need default value to pass the compile.
    SpriteComponent(int width = 0, int height = 0)
        : width(width), height(height){};
};

#endif
