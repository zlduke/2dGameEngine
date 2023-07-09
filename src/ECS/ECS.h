#ifndef ECS_H
#define ECS_H
#include <vector>
#include <bitset>

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

class Registry
{
};

// Typically we define functions with template in .h file
template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

#endif
