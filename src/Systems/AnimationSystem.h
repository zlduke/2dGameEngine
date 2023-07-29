#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem : public System
{
public:
    AnimationSystem()
    {
        RequireComponent<AnimationComponent>();
        RequireComponent<SpriteComponent>();
    }
    void Update()
    {
        std::vector<Entity> entities = GetSystemEntites();
        for (auto entity : entities)
        {
            auto &animation = entity.GetComponent<AnimationComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            animation.currentFrame = (SDL_GetTicks() - animation.startTime) *
                                     animation.frameSpeedRate / 1000 % animation.numFrames;
            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }
};

#endif
