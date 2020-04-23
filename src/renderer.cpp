#include "renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>


Renderer::Renderer(Shader *shaderProgram):
    _shaderProgram(shaderProgram)
{
}


void Renderer::drawObject(Kobject kobject, glm::vec2 position, glm::vec2 size, GLfloat rotate)
{
    GLint vertexColorLocation = glGetUniformLocation(_shaderProgram->ID, "color");
    glUniform4f(vertexColorLocation,
                glm::sin(glfwGetTime()),
                glm::cos(2 * position[1] / 200),
                glm::cos(3 * glfwGetTime()),
                1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(position,  0.0f));
    _shaderProgram->SetMatrix4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(size, 0));
    _shaderProgram->SetMatrix4("model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
