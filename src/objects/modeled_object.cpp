#include "object.h"

#include "camera.h"


ModeledObject::ModeledObject(GLfloat screen_scale,
                             Shader * shader_program,
                             Model * model,
                             Camera * camera,
                             const glm::vec3 * light_point,
                             glm::vec3 position,
                             glm::vec3 size):
        Object(screen_scale, shader_program, camera, light_point, position, size),
        model_(model)
{
    _mainObjectRotation = glm::mat4(1.0f);
}


void ModeledObject::Render()
{
    glUseProgram(shader_program_->program_ID_);
    shader_program_->SetVector3f("light.position", *light_point_);
    shader_program_->SetVector3f("viewPos", camera_->position_);

    // light properties
    GLfloat time = (float)glfwGetTime();
    shader_program_->SetVector3f("light.ambient", 1.f, 1.f, 1.f);
    shader_program_->SetVector3f("light.diffuse", 0.1f, cos(2*time), sin(time));
    shader_program_->SetVector3f("light.specular", 1.0f, .0f, .0f);

    // material properties
    shader_program_->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    shader_program_->SetFloat("material.shininess", 256.0f);


    glm::mat4 model = glm::mat4(1.0f);

//    GLfloat time = (float)glfwGetTime();
    model = glm::translate(model, stickedToPosition_);
    model *= _mainObjectRotation;
    model = glm::translate(model, position);
    model = glm::scale(model, size);

    shader_program_->SetMatrix4("model", model);
    glm::mat4 view = camera_->GetViewMatrix();
    shader_program_->SetMatrix4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(camera_->zoom_), screen_scale_, 0.1f, 1200.0f);
    shader_program_->SetMatrix4("projection", projection);

    glBindVertexArray(VAO_);
    model_->Draw(*shader_program_);
}


void ModeledObject::SetMainObjectRotation(glm::mat4 rotation){
    this->_mainObjectRotation = rotation;
}


void ModeledObject::SetStickedToPosition(glm::vec3 position){
    this->stickedToPosition_ = position;
}


void ModeledObject::DoCollisions()
{}
