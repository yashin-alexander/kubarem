#include "object.h"

#include "camera.h"


ModeledObject::ModeledObject(GLfloat screen_scale, Shader *shader_program, Model *model, Camera *camera,
                             glm::vec3 position, glm::vec3 size) :
        Object(screen_scale, shader_program, camera, position, size),
        model_(model) {
    _mainObjectRotation = glm::mat4(1.0f);
    shader_program_->SetInteger("light.shadowMap", 4);
}


void ModeledObject::Render() {
    glUseProgram(shader_program_->program_ID_);
    shader_program_->SetVector3f("viewPos", camera_->position_);

    // light properties
    GLfloat time = (float) glfwGetTime();
    shader_program_->SetVector3f("light.ambient", 0.1f, 0.1f, 0.1f);

    glm::vec3 colour(0.3f, 0.3f + (cos(2 * time) + 1.0f) * 0.7f, 0.3f + (1.0f + sin(time)) * 0.7f);
    shader_program_->SetVector3f("light.diffuse", colour);
    shader_program_->SetVector3f("light.specular", colour);

    // material properties
    shader_program_->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    shader_program_->SetFloat("material.shininess", 256.0f);


    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, stickedToPosition_);
    model *= _mainObjectRotation;
    model = glm::translate(model, position);
    model = glm::scale(model, size);

    glm::mat4 view = camera_->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera_->zoom_), screen_scale_, 0.1f, 1200.0f);

    shader_program_->SetMatrix4("model", model);
    shader_program_->SetMatrix4("view", view);
    shader_program_->SetMatrix4("projection", projection);

    glm::vec3 eye(250.0f, 500.0f, 700.0f), center(50.0f, 0.0f, 150.0f);
    glm::mat4 light_view = glm::lookAt(eye, center, camera_->world_up_);
    glm::mat4 light_projection = glm::ortho(-600.0f, 600.0f, -600.0f, 600.0f, 400.0f, 1500.0f);

    shader_program_->SetMatrix4("lightMVP", light_projection * light_view * model);
    shader_program_->SetVector3f("light.direction", eye - center);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, camera_->shadow_map_texture_);

    model_->Draw(*shader_program_);
}


void ModeledObject::SetMainObjectRotation(glm::mat4 rotation) {
    this->_mainObjectRotation = rotation;
}


void ModeledObject::SetStickedToPosition(glm::vec3 position) {
    this->stickedToPosition_ = position;
}


void ModeledObject::DoCollisions()
{}
