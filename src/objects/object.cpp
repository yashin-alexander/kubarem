#include "object.h"


Object::Object(GLfloat screen_scale, Shader *shader_program, Camera *camera, glm::vec3 position, glm::vec3 size) :
        screen_scale_(screen_scale),
        shader_program_(shader_program),
        camera_(camera),
        position(position),
        size(size)
        {}


Object::~Object()
{}


void Object::Render()
{}


void Object::DoCollisions()
{}

Shader *Object::GetShader() const {
    return shader_program_;
}

void Object::SetShader(Shader *new_shader) {
    assert(new_shader != nullptr);
    shader_program_ = new_shader;
}
