#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

class MovementSystem : public System
{
public:
    MovementSystem(){
        // only want certain entities
        // RequireComponent<TransformerComponent>();
        // RequireComponent<...>();
    };
    void Update()
    {
        // TODO:
        for (auto entity : GetEntities())
        {
            // Update the positions of the entities based on the velocities
            // per frame of game loop
        }
    }
};

#endif
