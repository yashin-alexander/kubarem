#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <stdio.h>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/rotate_vector.hpp>

#include "log.h"
#include "model.h"
#include "shader.h"
#include "object.h"


Object::Object(Model *model,
       Shader *shader,
       GLfloat screenScale,
       glm::vec3 size,
       glm::vec3 position):
  _shaderProgram(shader),
  _screenScale(screenScale),
  size(size),
  position(position)
{
    _model = model;
    _mainObjectRotation = glm::mat4(1.0f);
}


void Object::Render(GLint VAO, glm::vec2 main_size, Camera *camera)
{
    glm::mat4 model = glm::mat4(1.0f);

    GLfloat time = (float)glfwGetTime();
    model = glm::translate(model, stickedToPosition);
    model *= _mainObjectRotation;
    model = glm::translate(model, position);
    model = glm::scale(model, size);

    _shaderProgram->SetMatrix4("model", model);
    glm::mat4 view = camera->GetViewMatrix();
    _shaderProgram->SetMatrix4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), _screenScale, 0.1f, 1200.0f);
    _shaderProgram->SetMatrix4("projection", projection);

    glBindVertexArray(VAO);
    _model->Draw(*_shaderProgram);
}


void Object::SetMainObjectRotation(glm::mat4 rotation){
    this->_mainObjectRotation = rotation;
}
