#pragma once

#include <list>
#include "shader.h"
#include "camera.cpp"


class ParticleRenderer
{
public:
    ParticleRenderer(GLfloat screenScale, Shader *shaderProgram);
    ~ParticleRenderer() = default;

    void render(Camera *camera, const std::vector<Particle>& particles) ;
    void updateModelViewMatrix(glm::vec3 position, GLfloat rotation, GLfloat scale, glm::mat4 view);

protected:
        static constexpr GLfloat particle_vertices_[8] = {
                0.5f,  0.5f,
                0.5f, -0.5f,
                -0.5f, -0.5f,
                -0.5f,  0.5f
    };

    Shader *shaderProgram_;
    glm::mat4 modelViewMatrix_;
    GLfloat screenScale_;
    GLuint VAO_;
    GLuint VBO_;
};

