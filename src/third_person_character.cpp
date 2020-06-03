#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "log.h"
#include "model.h"
#include "shader.h"
#include "camera.cpp"


class Planet
{
private:
    GLfloat _rotationalSpeed;
    GLfloat _screenScale;
    GLfloat _axisTilt;
    GLfloat _orbitRadius;
    Shader *_shaderProgram = nullptr;
    Model  *_model = nullptr;

public:
    glm::vec2 position;
    glm::vec3 size;

    Planet(const char * modelPath,
           const char * texturePath,
           Shader *shader,
           GLfloat screenScale,
           GLfloat orbitRadius,
           GLfloat rotational_speed,
           GLfloat axisTilt,
           glm::vec3 planetSize):
      _shaderProgram(shader),
      _screenScale(screenScale),
      _orbitRadius(orbitRadius),
      _rotationalSpeed(rotational_speed),
      _axisTilt(axisTilt),
      size(planetSize)
    {
        _model = new Model(modelPath, false, texturePath);
//        _model = new Model(modelPath, false);
    }

    void Render(GLint VAO, glm::vec2 circling_around, glm::vec2 main_size, Camera *camera)
    {

        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), _screenScale, 0.1f, 500.0f);
//        projection = glm::perspective(glm::radians(90.0f), _screenScale, 0.1f, 100.0f);
        _shaderProgram->SetMatrix4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        GLfloat time = (float)glfwGetTime();
        position = glm::vec2(circling_around +
                             main_size +
                             glm::vec2(_orbitRadius * glm::vec2(sin(_rotationalSpeed * time), cos(_rotationalSpeed * time))));

        model = glm::rotate(model, _axisTilt, glm::vec3(0.0, 0.0, 1.0f)); // setup axis tilt
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, size);
        _shaderProgram->SetMatrix4("model", model);


        glm::mat4 view = camera->GetViewMatrix();
        view = glm::translate(view, glm::vec3(position[0], 0.0f, position[1]));
        _shaderProgram->SetMatrix4("view", view);

        glBindVertexArray(VAO);

        _model->Draw(*_shaderProgram);
    }
};
