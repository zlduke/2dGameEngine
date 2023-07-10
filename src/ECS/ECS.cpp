#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>

// TODO implement all the functions of ECS

int Entity::GetId() const
{
    return Entity::id;
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}
void System::RemoveEntityFromSystem(Entity entity)
{
    // implementation 1 - erase-remove idiom
    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  // [captured environment vars]
                                  [&entity](const Entity &other)
                                  { return entity == other; }),
                   entities.end());
}
std::vector<Entity> System::GetSystemEntites() const
{
    return entities;
}
const Signature &System::GetComponentSignature() const
{
    return componentSignature;
}

Entity Registry::CreateEntity()
{
    int entityId = numEntities++;
    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::Update()
{
    // Add the entities awaiting to be created to the active systems
    // remove entities to be killed in the system
    // my thoughts: to register the component signatures and append the vector as well
    // handle signature
    // extend the sizes
    // clear the set buffer
}
