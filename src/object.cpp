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

public:
    glm::vec2 position;
    glm::vec3 size;

    Object(const char * modelPath,
           const char * texturePath,
           Shader *shader,
           GLfloat screenScale,
           glm::vec3 planetSize,
           glm::vec2 position):
      _shaderProgram(shader),
      _screenScale(screenScale),
      size(planetSize),
      position(position)
    {
        _model = new Model(modelPath, false, texturePath);
//        _model = new Model(modelPath, false);
    }

    void Render(GLint VAO, glm::vec2 circling_around, glm::vec2 main_size, Camera *camera)
    {

        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), _screenScale, 0.1f, 500.0f);
        _shaderProgram->SetMatrix4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        GLfloat time = (float)glfwGetTime();

        model = glm::rotate(model, 1.55f, glm::vec3(0.0, 0.0, 1.0f)); // setup axis tilt
        model = glm::rotate(model, time, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, size);
        _shaderProgram->SetMatrix4("model", model);


        glm::mat4 view = camera->GetViewMatrix();
//        view = glm::translate(view, glm::vec3(0.0f, 10.f*sin(5 * time), 0.0f));
        view = glm::translate(view, glm::vec3(position[0], 0.0f, position[1]));
        _shaderProgram->SetMatrix4("view", view);

        glBindVertexArray(VAO);

        _model->Draw(*_shaderProgram);
    }
};
