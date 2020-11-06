#pragma once

#include "camera.cpp"
#include "particles/particle.h"
#include "particles/particle_renderer.h"

class ParticleController{

public:
    ParticleController(ParticleParameters parameters, uint32_t particles_number, GLfloat screenScale, Shader *shaderProgram);
    ~ParticleController();

    void update(GLfloat deltaTime);
    void renderParticles(Camera *camera);

private:
    ParticleParameters randomizeParticleParameters();

    ParticleParameters referenceParameters;
    ParticleRenderer *renderer;
    std::vector<Particle> particles;
};
