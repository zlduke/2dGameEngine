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
        // z-ordering -- this implementation puts the sorting per render frame
        // another school of ECS sorts/inserts the new entity into the list at right location
        std::vector<Entity> entities = GetSystemEntites();
        std::sort(entities.begin(), entities.end(),[](const Entity &e1, const Entity &e2) {
            return e1.GetComponent<SpriteComponent>().zIndex < e2.GetComponent<SpriteComponent>().zIndex;
        });

        // loop all entities
        for (auto entity : entities)
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
