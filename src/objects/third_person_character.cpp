#include "object.h"


ThirdPersonCharacter::ThirdPersonCharacter(
        GLfloat screen_scale,
        Shader *shader_program,
        Model *model,
        ThirdPersonCamera *camera,
        glm::vec3 size
) :
        ModeledObject(screen_scale, shader_program, model, nullptr, glm::vec3(0), size) {
    camera_ = camera;
    rotation_keeper_.SetLastModelMatrix(glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1.0, 0)));
    glGenVertexArrays(1, &VAO_);
}

glm::vec3 ThirdPersonCharacter::GetFront() const{
    return glm::vec3(((ThirdPersonCamera *) camera_)->front_[0],
                     0.0f,
                     ((ThirdPersonCamera *) camera_)->front_[2]);
}

glm::vec3 ThirdPersonCharacter::GetRight() const{
    return ((ThirdPersonCamera *) camera_)->right_;
}


void ThirdPersonCharacter::ProcessKeyboard(CameraMovement direction, float delta_time) {
    float velocity = delta_time * speed_ * 120;
    glm::vec3 new_position;

    if (direction == CameraMovement::kForward) {
        new_position = position + this->GetFront() * velocity;
        rotation_keeper_.SetMoveForward();
    }
    if (direction == CameraMovement::kBackward) {
        new_position = position - this->GetFront() * velocity;
        rotation_keeper_.SetMoveBackward();
    }
    if (direction == CameraMovement::kLeft) {
        new_position = position - this->GetRight() * velocity;
        rotation_keeper_.SetMoveLeft();
    }
    if (direction == CameraMovement::kRight) {
        new_position = position + this->GetRight() * velocity;
        rotation_keeper_.SetMoveRight();
    }

    this->MoveEvent(new_position);
}

void ThirdPersonCharacter::MoveEvent(glm::vec3 new_position) {
    this->position = new_position;
    ((ThirdPersonCamera *) this->camera_)->SetMainCharacterPosition(position);
}

void ThirdPersonCharacter::Render(glm::vec3 light_point) {
    glUseProgram(shader_program_->program_ID_);
    this->SetupLightning_(light_point);

    shader_program_->SetVector3f("viewPos", ((ThirdPersonCamera *) camera_)->position_);

    glm::mat4 projection = glm::perspective(glm::radians(45.f), screen_scale_, 0.1f, 500.0f);
    shader_program_->SetMatrix4("projection", projection);

    this->ProcessRotation();

    glm::mat4 view = ((ThirdPersonCamera *) camera_)->GetViewMatrix();
    view = glm::translate(view, position);
    shader_program_->SetMatrix4("view", view);

    glBindVertexArray(VAO_);
    model_->Draw(*shader_program_);
}

void ThirdPersonCharacter::ProcessRotation() {
    model_matrix = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1.0, 0));
    GLfloat rotation_delta = GetRotationMultiplier();

    if (rotation_keeper_.IsMovingBothDirections()) {
        model_matrix = glm::rotate(model_matrix,
                                   rotation_keeper_.GetFrontDirection() *
                                   rotation_keeper_.CalculateFrontRotation(rotation_delta),
                                   rotation_keeper_.GetFrontDirection() * GetFront() +
                                   rotation_keeper_.GetRightDirection() * GetRight());
        rotation_keeper_.SetLastModelMatrix(model_matrix);
    } else if (rotation_keeper_.GetFrontDirection()) {
        model_matrix = glm::rotate(model_matrix,
                                   rotation_keeper_.CalculateFrontRotation(rotation_delta),
                                   GetFront());
        rotation_keeper_.SetLastModelMatrix(model_matrix);
    } else if (rotation_keeper_.GetRightDirection()) {
        model_matrix = glm::rotate(model_matrix,
                                   rotation_keeper_.CalculateRightRotation(rotation_delta),
                                   GetRight());
        rotation_keeper_.SetLastModelMatrix(model_matrix);
    } else {
        model_matrix = rotation_keeper_.GetLastModelMatrix();
    }

    this->ProcessStickedObjects(model_matrix);
    rotation_keeper_.FlushDirections();

    model_matrix = glm::scale(model_matrix, size);
    shader_program_->SetMatrix4("model", model_matrix);
}

GLfloat ThirdPersonCharacter::GetRotationMultiplier() {
    if (time_ == 0.0f)
        time_ = (float) glfwGetTime();

    GLfloat old_time = time_;
    time_ = (float) glfwGetTime();
    return 9 * (time_ - old_time);
}

void ThirdPersonCharacter::ProcessStickedObjects(glm::mat4 model) {
    for (int i = 0; i < objects_sticked_; i++) {
        sticked_[i]->SetStickedToPosition(position);
        sticked_[i]->SetMainObjectRotation(model);
    }
}

void ThirdPersonCharacter::DoCollisions(ModeledObject **objects_list, GLint size) {
    ModeledObject *current_object = nullptr;
    GLfloat distance;

    for (int i = 0; i <= size; i++) {
        current_object = objects_list[i];
        distance = glm::distance(current_object->position, position);
        if (!current_object->isSticked
            && distance <= (this->size[0] + current_object->size[0])) {
            log_info("New object found!");
            log_info("Distance to object: %f", distance);

            this->sticked_[objects_sticked_] = current_object;
            objects_sticked_++;
            current_object->isSticked = true;
            glm::vec3 new_object_position = position - current_object->position;

            glm::mat4 r = glm::mat4(1);
            r = glm::rotate(r, glm::radians(90.0f), glm::vec3(0, 1.0, 0));

            new_object_position = glm::vec3(model_matrix * glm::vec4(glm::normalize(new_object_position), 1.0f));
            log_info("New object position %f %f %f", new_object_position[0], new_object_position[1],
                     new_object_position[2]);
            new_object_position = glm::vec3(r * glm::vec4(new_object_position, 1.0f));
            log_info("New object position %f %f %f", new_object_position[0], new_object_position[1],
                     new_object_position[2]);

            current_object->position = new_object_position;
        } else {
        }
    }
}

