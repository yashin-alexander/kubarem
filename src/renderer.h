#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "object.h"


class Renderer
{
private:
    Shader *_shaderProgram;
public:
    Renderer(Shader *shaderProgram);
    void drawObject(Object kobject, glm::vec2 postition, glm::vec2 size, GLfloat rotate);
};

#endif // RENDERER_H
