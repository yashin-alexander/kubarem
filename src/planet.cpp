#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "model.h"
#include "shader.h"


class Planet
{
private:
    GLfloat _rotationalSpeed;
    GLfloat _screenScale;
    GLfloat _axisTilt;
    Shader *_shaderProgram = nullptr;
    Model  *_model = nullptr;

public:
    Planet(const char * modelPath, Shader *shader, GLfloat screenScale, GLfloat rotational_speed, GLfloat axisTilt):
      _shaderProgram(shader),
      _screenScale(screenScale),
      _rotationalSpeed(rotational_speed),
      _axisTilt(axisTilt)
    {
        _model = new Model(modelPath);
    }

    void Render(GLint VAO)
    {
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        view  = glm::translate(view, glm::vec3(0.0f, 1.0f, -5.0f));
        GLfloat time = (float)glfwGetTime();
        model = glm::rotate(model, _axisTilt, glm::vec3(0.0, 0.0, 1.0f)); // setup axis tilt
        model = glm::rotate(model, _rotationalSpeed * time, glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(glm::radians(120.0f), _screenScale, 0.1f, 100.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 0.8f, 1.0f));

        _shaderProgram->SetMatrix4("projection", projection);
        _shaderProgram->SetMatrix4("view", view);
        _shaderProgram->SetMatrix4("model", model);

        glBindVertexArray(VAO);

        _model->Draw(*_shaderProgram);
    }
};
