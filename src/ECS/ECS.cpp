#include "ECS.h"

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
    if (entityId > entityComponentSignatures.size())
    {
        entityComponentSignatures.resize(entityId);
    }
    // Q: when to handle the signatures?
    entitiesToBeAdded.insert(entity);
}
