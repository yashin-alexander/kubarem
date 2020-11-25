#include "objects/light_object.h"

#include "shader.h"
#include "objects/object.h"

LightObject::LightObject() {
    glGenFramebuffers(1, &depth_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_buffer_);

    glGenTextures(1, &depth_texture_);
    glBindTexture(GL_TEXTURE_2D, depth_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, size_, size_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_, 0);
    glDrawBuffer(GL_NONE);

    depth_shader_ = Shader::LoadFromFile("src/shaders/shadowmap_vs.glsl", "src/shaders/shadowmap_fs.glsl");
}

LightObject::~LightObject() {
    glDeleteFramebuffers(1, &depth_buffer_);
    glDeleteTextures(1, &depth_texture_);
    delete depth_shader_;
}

void LightObject::Render(Object **objects_to_render, unsigned objects_amount) {
    glBindFramebuffer(GL_FRAMEBUFFER, depth_buffer_);
    glViewport(0, 0, size_, size_);

    glClear(GL_DEPTH_BUFFER_BIT);

    for (unsigned i = 0; i < objects_amount; ++i) {
        Object *object = objects_to_render[i];
        Shader *object_shader = object->GetShader();
        object->SetShader(depth_shader_);
        object->Render();
        object->SetShader(object_shader);
    }
}
