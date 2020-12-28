#include "object.h"


Object::Object(GLfloat screen_scale,
               Shader *shader_program,
               Camera *camera,
               glm::vec3 position,
               glm::vec3 size) :
        screen_scale_(screen_scale),
        shader_program_(shader_program),
        camera_(camera),
        position(position),
        size(size) {}


void Object::DoCollisions() {}


void Object::SetupLightning_(glm::vec3 light_point) {
    shader_program_->SetVector3f("light.position", light_point);

    // light properties
    auto time = (GLfloat) glfwGetTime();
    shader_program_->SetVector3f("light.ambient", 1.f, 1.f, 1.f);
    shader_program_->SetVector3f("light.diffuse", 0.1f, cos(2 * time), sin(time));
    shader_program_->SetVector3f("light.specular", 1.0f, .0f, .0f);

    // material properties
    shader_program_->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    shader_program_->SetFloat("material.shininess", 256.0f);
}