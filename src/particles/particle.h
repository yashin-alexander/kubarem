#pragma once

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

struct ParticleParameters {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    GLfloat gravity_effect;
    GLfloat life_length;
    GLfloat rotation;
    GLfloat scale;
};

class Particle
{
public:
    Particle(ParticleParameters parameters);
    ~Particle() = default;

    void setParameters(ParticleParameters parameters);

    const glm::vec3 &getPosition() const;

    GLfloat getRotation() const;

    GLfloat getScale() const;

    glm::vec4 getColor() const;

    void update(GLfloat delta_time);

    GLboolean isActive() const;

protected:
    ParticleParameters parameters;

    GLfloat elapsed_time = 0;
};

