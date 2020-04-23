#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


class Kobject
{
public:
    glm::vec2   Position, Size;
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;
    Kobject();
    Kobject(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f));

    void Init();
};


#endif // OBJECT_H
