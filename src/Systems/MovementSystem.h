#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <string>

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<RigidBodyComponent>();
        RequireComponent<TransformComponent>();
    };
    void Update(double deltaTime)
    {
        for (auto entity : GetSystemEntites())
        {
            const auto &rigidBody = entity.GetComponent<RigidBodyComponent>();
            auto &transform = entity.GetComponent<TransformComponent>();

            transform.position.x += rigidBody.velocity.x * deltaTime;
            transform.position.y += rigidBody.velocity.y * deltaTime;
        }
    }
};

#endif
