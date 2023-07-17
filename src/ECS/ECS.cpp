#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>

int IComponent::nextId = 0;

int Entity::GetId() const
{
    return Entity::id;
}

void System::AddEntityToSystem(Entity entity)
{
    // where to perform filtering?
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
    entity.registry = this;
    entitiesToBeAdded.insert(entity);
    if (entityId >= entityComponentSignatures.size())
    {
        entityComponentSignatures.resize(entityId + 1);
    }
    Logger::Log("Entity created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::Update()
{
    // Add the entities awaiting to be created to the active systems
    // loop entities and insert into correct systems
    for (auto entity : entitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    // remove entities waiting to be killed
}

void Registry::AddEntityToSystems(Entity entity)
{
    const auto entityId = entity.GetId();
    const auto &entityComponentSignature = entityComponentSignatures[entityId];

    // loop and test signatures
    for (auto &pair : systems)
    {
        auto const &systemComponentSignature = pair.second->GetComponentSignature();
        bool isInterested = (systemComponentSignature & entityComponentSignature) == systemComponentSignature;
        if (isInterested)
        {
            pair.second->AddEntityToSystem(entity);
        }
    }
}
