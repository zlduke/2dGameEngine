#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent
{
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;

    // notice the confusingly identical names here (args vs. member vars)
    // another way to get around that is by using this->MEMBER_VAR to differentiate
    TransformComponent(glm::vec2 position = glm::vec2(0, 0),
                       glm::vec2 scale = glm::vec2(1, 1),
                       double rotation = 0.0)
        : position(position), scale(scale), rotation(rotation){};
};

#endif