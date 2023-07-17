#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include "../AssetStore/AssetStore.h"

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    };
    void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore)
    {
        for (auto entity : GetSystemEntites())
        {
            const auto &transform = entity.GetComponent<TransformComponent>();
            const auto &sprite = entity.GetComponent<SpriteComponent>();
            SDL_Texture *texture = assetStore->GetTexture(sprite.assetId);
            // set the source rectangle and position rectangle for the texture
            SDL_Rect srcRect = sprite.srcRect; // used to crop the source
            SDL_Rect dstRect = {static_cast<int>(transform.position.x),
                                static_cast<int>(transform.position.y),
                                static_cast<int>(sprite.width * transform.scale.x),
                                static_cast<int>(sprite.height * transform.scale.y)};
            // send portion of a texture to a portion of the target renderer
            SDL_RenderCopyEx(renderer, texture,
                             &srcRect,
                             &dstRect,
                             transform.rotation,
                             NULL,
                             SDL_FLIP_NONE);
        }
    }
};

#endif
