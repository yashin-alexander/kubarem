#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <stdio.h>
#include "glm/gtx/string_cast.hpp"

#include "log.h"
#include "model.h"
#include "shader.h"
#include "camera.cpp"


class ThirdPersonCharacter
{
private:
    GLfloat _screenScale;
    Shader *_shaderProgram = nullptr;
    Model  *_model = nullptr;
    glm::vec3 _front;
    glm::vec3 _right;
    ThirdPersonCamera *_camera = nullptr;

public:
    glm::vec3 position;
    glm::vec3 size;

    ThirdPersonCharacter(const char * modelPath,
           const char * texturePath,
           Shader *shader,
           GLfloat screenScale,
           glm::vec3 planetSize,
           ThirdPersonCamera * camera):
      _shaderProgram(shader),
      _screenScale(screenScale),
      size(planetSize),
      _camera(camera)
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        _model = new Model(modelPath, false, texturePath);
        _front = glm::vec3(0.0f, 0.0f, -1.0f);
        _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    }


    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {

        float velocity = deltaTime * 60;
        if (direction == FORWARD)
            position += _front * velocity;
        if (direction == BACKWARD)
            position -= _front * velocity;
        if (direction == LEFT)
            position -= _right * velocity;
        if (direction == RIGHT)
            position += _right * velocity;

//        this->_camera->setPosition(position);
    }

    void Render(GLint VAO, glm::vec2 circling_around, glm::vec2 main_size)
    {

        glm::mat4 projection = glm::perspective(glm::radians(45.f), _screenScale, 0.1f, 500.0f);
        _shaderProgram->SetMatrix4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        GLfloat time = (float)glfwGetTime();

        model = glm::rotate(model, 1.55f, glm::vec3(0.0, 0.0, 1.0f)); // setup axis tilt
        model = glm::rotate(model, time, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, size);
        _shaderProgram->SetMatrix4("model", model);

        glm::mat4 view = _camera->GetViewMatrix();

        view = glm::translate(view, position);
        _shaderProgram->SetMatrix4("view", view);

        glBindVertexArray(VAO);
        _model->Draw(*_shaderProgram);
    }
};
