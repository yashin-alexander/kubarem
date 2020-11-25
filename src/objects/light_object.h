#pragma once

#include "glad/glad.h"

class Object;

class Shader;

class LightObject final {

public:
    LightObject();

    ~LightObject();

    void Render(Object **objects_to_render, unsigned objects_amount);

    GLuint GetDepthTexture() const { return depth_texture_; }

private:
    GLuint depth_buffer_ = 0;
    GLuint depth_texture_ = 0;
    Shader *depth_shader_ = nullptr;

    const int size_ = 1024 * 8;
};


