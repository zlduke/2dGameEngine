#ifndef ECS_H
#define ECS_H
#include <vector>
#include <bitset>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;
///////////////////////////////////////////////////////////////
// Signature
///////////////////////////////////////////////////////////////
// We use a bitset to keep track of what components an entity
// has, which effectively defines an entity. Such definition
// (signature) helps the system to find the required entites
// bearing the target signature.
///////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

// naming convention: we can think of the BaseComponent as interface
struct IComponent
{
protected:
    static int nextId;
};

// we want to return an unique id per TYPE of component
// but remain the same id across multiple instances of the same TYPE
template <typename TComponent>
class Component : public IComponent
{
public:
    // 'static' ensures that we can operate on the class level, not necessarily on the object.
    static int GetId()
    {
        static auto id = nextId++;
        return id;
    }
};

// forward declaration before using it in the Entity class
class Entity
{
private:
    int id;

public:
    Entity(int id) : id(id){};
    Entity(const Entity &other) = default;
    int GetId() const; // encapsulation trick; const = not changing anything
    Entity &operator=(const Entity &other) = default;
    bool operator==(const Entity &other) const { return id == other.GetId(); }
    bool operator!=(const Entity &other) const { return id != other.GetId(); }
    bool operator>(const Entity &other) const { return id > other.GetId(); }
    bool operator<(const Entity &other) const { return id < other.GetId(); }

    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs &&...args);
    template <typename TComponent>
    void RemoveComponent();
    template <typename TComponent>
    bool HasComponent() const;
    template <typename TComponent>
    TComponent &GetComponent() const;
    // Hold a pointer to the entity's owner registry
    class Registry *registry; // forward declaration
};

class System
{
private:
    Signature componentSignature;
    std::vector<Entity> entities;

public:
    System() = default;
    ~System() = default;
    void AddEntityToSystem(Entity entity);
    // TODO: how to remove a specific one??
    // by for-loop and matching examination?
    // When would we usually remove entities?
    // and by what quantities?
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntites() const;
    const Signature &GetComponentSignature() const;

    // Defines the component that the entities must have to be considered by this system
    template <typename TComponent>
    void RequireComponent();
};

// Typically we define functions with template in .h file
template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

////////////////////////////////////////////////////
// Pool
////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of
// objects of type T
////////////////////////////////////////////////////
class IPool
{
public:
    // enforcing this class to be only used by inheritence
    virtual ~IPool(){};
};

template <typename T>
class Pool : public IPool
{
private:
    std::vector<T> data;

public:
    Pool(int size = 100)
    {
        data.resize(size);
    }
    ~Pool() = default;
    bool isEmpty() const { return data.empty(); }
    int GetSize() const
    {
        return data.size();
    }
    void Resize(int n)
    {
        return data.resize(n);
    }
    void Add(T object)
    {
        data.push_back(object);
    }
    void Set(int index, T object)
    {
        data[index] = object;
    }
    T &Get(int index)
    {
        // why is this needed?
        return static_cast<T &>(data[index]);
    }
    T &operator[](unsigned int index)
    {
        return static_cast<T &>(data[index]);
    }
};
////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////
// aka "coordinator", "world class", "enetity manager"
// The registry manages the creation and destruction
// of entities, add systems, and components.
////////////////////////////////////////////////////
class Registry
{
private:
    int numEntities = 0;

    // Avoid adding entites in the middle of something else.
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

    /* componentPools: vector of pools
    component types [0][1] ...
    ---------------------------------
    for entity0     [x][z] ...
    for entity1     [y][w] ...
    ...             ...
    ---------------------------------
    */
    // use base dummy IPool to avoid specifying pool<T>.
    // allowing different types of Pools to be mixed together
    std::vector<std::shared_ptr<IPool>> componentPools;

    // vector[entity id] = componentSignture of the entity
    std::vector<Signature> entityComponentSignatures;

    // here we don't need to sort the maps
    // Given typeinfo(class) we can find the actual instantiation
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
    Registry() = default;

    void Update();

    Entity CreateEntity();
    void AddEntityToSystem(Entity entity);

    // TODO: AddComponent<TComponent>(...) generalize types of components
    // and ways (args) to initialize them. For examples:
    // registry.AddComponent<TransformComponent>(entity, locaiton, speed, rottation)
    // registry.AddComponent<SpriteComponent>(entity, "path_to_sprite.png")
    template <typename TComponent, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&...args);
    template <typename TComponent>
    void RemoveComponent(Entity entity);
    template <typename TComponent>
    bool HasComponent(Entity entity) const;
    template <typename TComponent>
    TComponent &GetComponent(Entity entity) const;

    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs &&...args);
    template <typename TSystem>
    void RemoveSystem();
    template <typename TSystem>
    bool HasSystem() const;
    template <typename TSystem>
    TSystem &GetSystem() const;

    // Add entity to systems that are interested in such entity by signature
    void AddEntityToSystems(Entity entity);
};

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&...args)
{
    // Tsystem might be co-owned since this pointer is inserted to some other data structures.
    std::shared_ptr<TSystem> newSystemPtr = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert({std::type_index(typeid(TSystem)), newSystemPtr});
}

template <typename TSystem>
void Registry::RemoveSystem()
{
    // typically, try to find a pointer, of that key (stupid C++), then delete by key
    auto system = systems.find(std::type_index(typeid(TSystem)));
    // what if it doesn't exist
    systems.erase(system);
}
template <typename TSystem>
bool Registry::HasSystem() const
{
    auto it = systems.find(std::type_index(typeid(TSystem)));
    return it != systems.end();
}
template <typename TSystem>
TSystem &Registry::GetSystem() const
{
    auto it = systems.find(std::type_index(typeid(TSystem)));
    if (it == systems.end())
    {
        throw std::logic_error("System not found");
    }
    // ChatGPT: In C++, it's safe to cast the base pointer to a \
    // derived type as long as you're absolutely certain the base \
    // is pointing to a derived object.
    return *(std::static_pointer_cast<TSystem>(it->second));
}

//  "For all (...) the arguments (args) of arbitrary types (TArgs), \
// preserve their original value category (&&)".
template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args)
{
    // get ids and initialize the component
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    // Check if this component exists in the pool
    if (componentId >= componentPools.size())
    {
        componentPools.resize(componentId + 1, nullptr);
    }
    // Check the pool associated with the component
    if (!componentPools[componentId])
    {
        // using new to retain the ownership beyond this func
        // to be replaced by smart pointers
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }

    // Check the entity in that pool
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    if (entityId >= componentPool->GetSize())
    {
        // I want to use entityId + 1 instead of this
        componentPool->Resize(numEntities);
    }

    // create new component and register the entity-component pair
    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);

    // update the signature (bitset) for this entity
    // shouldn't this bracket be included?
    /*
    if (entityId >= entityComponentSignatures.size())
    {
        entityComponentSignatures.resize(entityId + 1);
    }
    */
    entityComponentSignatures[entityId].set(componentId);
    Logger::Log("Component id = " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();
    entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();
    return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent &Registry::GetComponent(Entity entity) const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();
    auto poolPtr = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return poolPtr->Get(entityId);
}

// Entitiy functions
template <typename TComponent, typename... TArgs>
void Entity::AddComponent(TArgs &&...args)
{
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}
template <typename TComponent>
void Entity::RemoveComponent()
{
    registry->RemoveComponent<TComponent>(*this);
}
template <typename TComponent>
bool Entity::HasComponent() const
{
    return registry->HasComponent<TComponent>(*this);
}
template <typename TComponent>
TComponent &Entity::GetComponent() const
{
    return registry->GetComponent<TComponent>(*this);
}
#endif
