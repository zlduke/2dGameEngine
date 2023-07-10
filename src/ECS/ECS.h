#ifndef ECS_H
#define ECS_H
#include <vector>
#include <bitset>
#include <unordered_map>
#include <typeindex>
#include <set>

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
    // 'static' ensures that we can operate on the class level, not necessarily on the object.
    static int GetId()
    {
        static auto id = nextId++;
        return id;
    }
};

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
};

//
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
    vector index = component type id
    pool index = entity id
    */
    // use base dummy IPool to avoid specifying pool<T>.
    // allowing different types of Pools to be mixed together
    std::vector<IPool *> componentPools;

    // vector[entity id] = componentSignture of the entity
    std::vector<Signature> entityComponentSignatures;

    // here we don't need to sort the maps
    // Given typeinfo(class) we can find the actual instantiation
    std::unordered_map<std::type_index, System *> systems;

public:
    Registry() = default;

    // Think of how to add E, C, S first
    Entity CreateEntity();

    // AddComponent()
    // GetComponent()

    // AddSystem()
};

#endif
