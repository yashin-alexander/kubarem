#pragma once

#include <math.h>
#include <stdio.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/string_cast.hpp"

#include "log.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "objects/object.h"


class MoveRotationKeeper
{

public:
    MoveRotationKeeper(){}

    void FlushDirections()
    {
        right_direction_ = 0.0f;
        front_direction_ = 0.0f;
    }

    void SetMoveRight()
    {
        right_direction_ = 1.0f;
        last_x_rotation_ = right_direction_;
    }

    void SetMoveLeft()
    {
        right_direction_ = -1.0f;
        last_x_rotation_ = right_direction_;
    }

    void SetMoveForward()
    {
        front_direction_ = 1.0f;
        last_z_rotation_ = front_direction_;
    }

    void SetMoveBackward()
    {
        front_direction_ = -1.0f;
        last_z_rotation_ = front_direction_;
    }

    void SetLastModelMatrix(glm::mat4 model)
    {
        this->model_ = model;
    }

    glm::mat4 GetLastModelMatrix()
    {
        return this->model_;
    }

    GLboolean IsMovingBothDirections()
    {
        return right_direction_ && front_direction_;
    }

    GLfloat GetFrontDirection()
    {
        return front_direction_;
    }

    GLfloat GetRightDirection()
    {
        return right_direction_;
    }

    GLfloat CalculateFrontRotation(GLfloat rotation_step){
        front_rotation_amount_ += front_direction_ * rotation_step;
        return front_rotation_amount_;
    }

    GLfloat CalculateRightRotation(GLfloat rotation_step){
        right_rotation_amount_ += right_direction_ * rotation_step;
        return right_rotation_amount_;
    }

private:
    GLfloat right_direction_ = 0.0f;
    GLfloat front_direction_ = 0.0f;

    GLfloat front_rotation_amount_ = 0.0f;
    GLfloat right_rotation_amount_ = 0.0f;

    GLfloat last_z_rotation_ = 0.0f;
    GLfloat last_x_rotation_ = 0.0f;

    glm::mat4 model_;
};


class ThirdPersonCharacter
{

public:
    ThirdPersonCamera *camera_ = nullptr;
    glm::vec3 position_;
    glm::vec3 size_;
    ModeledObject *sticked_[100];
    GLint objects_sticked_ = 0;

    ThirdPersonCharacter(const char *model_path,
           Shader *shader,
           GLfloat screen_scale,
           glm::vec3 size,
           ThirdPersonCamera *camera):
            shader_program_(shader),
            screen_scale_(screen_scale),
            size_(size),
            camera_(camera)
    {
        position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        model_ptr_ = new Model(model_path, false);

        rotation_keeper_.SetLastModelMatrix(
                glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1.0, 0)));
    }

    virtual ~ThirdPersonCharacter()
    {
        delete model_ptr_;
    }

    glm::vec3 GetFront(){
        return glm::vec3(camera_->front_[0],
                         0.0f,
                         camera_->front_[2]);
    }

    glm::vec3 GetRight(){
        return camera_->right_;
    }


    void ProcessKeyboard(CameraMovement direction, float delta_time)
    {
        float velocity = delta_time * speed_ * 120;
        glm::vec3 new_position;

        if (direction == CameraMovement::kForward){
            new_position = position_ + this->GetFront() * velocity;
            rotation_keeper_.SetMoveForward();
        }
        if (direction == CameraMovement::kBackward){
            new_position = position_ - this->GetFront() * velocity;
            rotation_keeper_.SetMoveBackward();
        }
        if (direction == CameraMovement::kLeft){
            new_position = position_ - this->GetRight() * velocity;
            rotation_keeper_.SetMoveLeft();
        }
        if (direction == CameraMovement::kRight){
            new_position = position_ + this->GetRight() * velocity;
            rotation_keeper_.SetMoveRight();
        }

        this->MoveEvent(new_position);
    }

    void MoveEvent(glm::vec3 new_position)
    {
//        if (isInsidePlate(newPosition)){
        this->position_ = new_position;
        this->camera_->SetMainCharacterPosition(position_);
//        } else {
//            if (abs(speed) < 8){
//                speed *= -1.0f;
//            }
//            else {
//                speed /= -1.0f;
//            }
//            log_info("Bounds touched! Controls inverted!");
//        }
    }

    GLboolean IsInsidePlate(glm::vec3 position_to_check)
    {
        // hardcode plate radius
        return (pow(position_to_check[0], 2) + pow(position_to_check[2], 2)) < 120000;
    }

    void Render(GLint VAO, glm::vec2 circling_around, glm::vec2 main_size)
    {
        glUseProgram(shader_program_->ID);
        glm::vec3 lightPos(60.f, 20.f, -60.f);
        shader_program_->SetVector3f("light.position", lightPos);
        shader_program_->SetVector3f("viewPos", camera_->position_);

        // light properties
        GLfloat time = (float)glfwGetTime();
        shader_program_->SetVector3f("light.ambient", 1.f, 1.f, 1.f);
        shader_program_->SetVector3f("light.diffuse", 0.1f, cos(2 * time), sin(time));
        shader_program_->SetVector3f("light.specular", 1.0f, .0f, .0f);

        // material properties
        shader_program_->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        shader_program_->SetFloat("material.shininess", 256.0f);


        glm::mat4 projection = glm::perspective(glm::radians(45.f), screen_scale_, 0.1f, 500.0f);
        shader_program_->SetMatrix4("projection", projection);

        this->ProcessRotation();

        glm::mat4 view = camera_->GetViewMatrix();
        view = glm::translate(view, position_);
        shader_program_->SetMatrix4("view", view);

        glBindVertexArray(VAO);
        model_ptr_->Draw(*shader_program_);
    }

    void ProcessRotation(){
        model_ = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1.0, 0));
        GLfloat rotation_delta = GetRotationMultiplier();

//        /*
        if (rotation_keeper_.IsMovingBothDirections())
        {
            model_ = glm::rotate(model_,
                                 rotation_keeper_.GetFrontDirection() *
                                 rotation_keeper_.CalculateFrontRotation(rotation_delta),
                                 rotation_keeper_.GetFrontDirection() * GetFront() +
                                 rotation_keeper_.GetRightDirection() * GetRight());
            rotation_keeper_.SetLastModelMatrix(model_);
        } else if (rotation_keeper_.GetFrontDirection()){
            model_ = glm::rotate(model_,
                                 rotation_keeper_.CalculateFrontRotation(rotation_delta),
                                 GetFront());
            rotation_keeper_.SetLastModelMatrix(model_);
        } else if (rotation_keeper_.GetRightDirection()){
            model_ = glm::rotate(model_,
                                 rotation_keeper_.CalculateRightRotation(rotation_delta),
                                 GetRight());
            rotation_keeper_.SetLastModelMatrix(model_);
        } else {
            model_ = rotation_keeper_.GetLastModelMatrix();
        }
//        */

        this->ProcessStickedObjects(model_);
        rotation_keeper_.FlushDirections();

        model_ = glm::scale(model_, size_);
        shader_program_->SetMatrix4("model", model_);
    }

    GLfloat GetRotationMultiplier(){
        if(time_ == 0.0f)
            time_ = (float)glfwGetTime();

        GLfloat old_time = time_;
        time_ = (float)glfwGetTime();
        return 9 * (time_ - old_time);
    }

    void ProcessStickedObjects(glm::mat4 model){
        for (int i = 0; i < objects_sticked_; i++){
            sticked_[i]->SetStickedToPosition(position_);
            sticked_[i]->SetMainObjectRotation(model);
        }
    }

    void DoCollisions(ModeledObject **objects_list, GLint size){
        ModeledObject *current_object = nullptr;
        GLfloat distance;

        for (int i = 0; i <= size; i++)
        {
            current_object = objects_list[i];
            distance = glm::distance(current_object->position, position_);
            if (!current_object->isSticked
                && distance <= (size_[0] + current_object->size[0]))
            {
                log_info("New object found!");
                log_info("Distance to object: %f", distance);

                this->sticked_[objects_sticked_] = current_object;
                objects_sticked_++;
                current_object->isSticked = true;
                glm::vec3 new_object_position  = position_ - current_object->position;
//                glm::vec3 newObjectPosition = glm::vec3(4, 0, 0);

                glm::mat4 r = glm::mat4(1);
                r = glm::rotate(r, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
//                r = glm::rotate(r, rotationKeeper->getFrontDirection() * rotationKeeper->calculateFrontRotation(0.0f),
//                                rotationKeeper->getFrontDirection() * _getFront());

                new_object_position = glm::vec3(model_ * glm::vec4(glm::normalize(new_object_position), 1.0f));
                log_info("New object position %f %f %f", new_object_position[0], new_object_position[1], new_object_position[2]);
                new_object_position = glm::vec3(r * glm::vec4(new_object_position, 1.0f));
                log_info("New object position %f %f %f", new_object_position[0], new_object_position[1], new_object_position[2]);
//                newObjectPosition = glm::vec3(model * glm::vec4(glm::normalize(newObjectPosition), 1.0f));

                current_object->position = new_object_position;
            } else {
            }
        }
    }

private:
    GLfloat screen_scale_;
    Model  *model_ptr_ = nullptr;
    glm::mat4 model_;
    Shader *shader_program_ = nullptr;

    GLfloat speed_ = 1.0f;
    GLfloat time_ = 0.0f;

    MoveRotationKeeper rotation_keeper_;
};

