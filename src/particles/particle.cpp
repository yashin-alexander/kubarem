#include "particles/particle.h"
#include "log.h"


Particle::Particle(ParticleParameters parameters)
{
    setParameters(parameters);
}


void Particle::setParameters(ParticleParameters new_parameters) {
    this->parameters = new_parameters;
    elapsed_time = 0;
}

const glm::vec3& Particle::getPosition() const {
    return parameters.position;
}

GLfloat Particle::getRotation() const {
    return parameters.rotation;
}

GLfloat Particle::getScale() const {
    return parameters.scale;
}

glm::vec4 Particle::getColor() const {
    return parameters.color;
}

void Particle::update(GLfloat delta_time) {
    if (elapsed_time < parameters.life_length) {
        // update rotation
        parameters.rotation += parameters.rotation * delta_time;

        // update velocity u = u1 + at;
        parameters.velocity.y = parameters.velocity.y - delta_time * parameters.gravity_effect;
        parameters.position += delta_time * parameters.velocity;

        // update alpha channel
        parameters.color.w = 1 - elapsed_time / parameters.life_length;

        elapsed_time += delta_time;
    }
}


GLboolean Particle::isActive() const {
    return elapsed_time < parameters.life_length;
}
