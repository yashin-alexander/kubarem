#pragma once

#include "particles/particle.h"
#include "particles/particle_renderer.h"

class BaseCamera;

class ParticleController{

public:
    ParticleController(ParticleParameters parameters, uint32_t particles_number);
    ~ParticleController();

    void update(GLfloat deltaTime);
    void renderParticles(BaseCamera *camera, Shader * shader, GLfloat screen_scale);
    int getParticlesNumber(){
        return this->particles.size();
    };

    ParticleParameters referenceParameters;
private:
    ParticleParameters randomizeParticleParameters();

    ParticleRenderer *renderer;
    std::vector<Particle> particles;
};
