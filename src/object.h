#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <stdio.h>
#include "glm/gtx/string_cast.hpp"

#include "log.h"
#include "model.h"
#include "shader.h"
#include "camera.cpp"


class Object
{
private:
    GLfloat _screenScale;
    Shader *_shaderProgram = nullptr;
    Model  *_model = nullptr;
    glm::mat4 _mainObjectRotation;
public:
    glm::vec3 position;
    glm::vec3 size;

    Object(const char * modelPath,
           const char * texturePath,
           Shader *shader,
           GLfloat screenScale,
           glm::vec3 planetSize,
           glm::vec3 position);

    virtual void Render(GLint VAO, glm::vec3 circling_around, glm::vec2 main_size, Camera *camera);
    void SetMainObjectRotation(glm::mat4 rotation);
};


#endif // OBJECT_H
