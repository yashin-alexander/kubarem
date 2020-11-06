#include "shader.h"
#include "particles/particle.h"
#include "particles/particle_renderer.h"


ParticleRenderer::ParticleRenderer(GLfloat screenScale, Shader *shaderProgram) : screenScale_(screenScale),
shaderProgram_(shaderProgram)
{
    glGenVertexArrays(1, &this->VAO_);
    glGenBuffers(1, &this->VBO_);
    glBindVertexArray(this->VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_vertices_), particle_vertices_, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}


void ParticleRenderer::render(Camera *camera, const std::vector<Particle>& particles) {
    shaderProgram_->Use();

    for (auto& it : particles) {
        if (it.isActive()) {
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), screenScale_, 0.1f, 1200.0f);
            updateModelViewMatrix(it.getPosition(), it.getRotation(), it.getScale(), camera->GetViewMatrix());
            shaderProgram_->SetMatrix4("projectionMatrix", projection);
            shaderProgram_->SetMatrix4("modelViewMatrix", modelViewMatrix_);
            glBindVertexArray(VAO_);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
        }
    }
}


void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, GLfloat rotation, GLfloat scale,
                                             glm::mat4 view) {
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, position);
    model[0][0] = view[0][0];
    model[0][1] = view[1][0];
    model[0][2] = view[2][0];
    model[1][0] = view[0][1];
    model[1][1] = view[1][1];
    model[1][2] = view[2][1];
    model[2][0] = view[0][2];
    model[2][1] = view[1][2];
    model[2][2] = view[2][2];
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, glm::vec3(scale, scale, scale));

    modelViewMatrix_ = view * model;
}