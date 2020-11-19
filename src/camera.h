#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "log.h"

enum class CameraMovement {
    kForward,
    kBackward,
    kLeft,
    kRight
};

const float kYaw         = -90.0f;
const float kPitch       =  0.0f;
const float kSpeed       =  80.0f;
const float kSensitivity =  0.1f;
const float kZoom        =  45.0f;


class Camera
{
public:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    float yaw_;
    float pitch_;

    float movement_speed_;
    float mouse_sensitivity_;
    float zoom_;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = kYaw, float pitch = kPitch)
           : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(kSpeed), mouse_sensitivity_(kSensitivity),
           zoom_(kZoom)
    {
        position_ = position;
        world_up_ = up;
        yaw_ = yaw;
        pitch_ = pitch;
        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position_, position_ + front_, up_);
    }

    void ProcessKeyboard(CameraMovement direction, float delta_time)
    {
        float velocity = movement_speed_ * delta_time;
        if (direction == CameraMovement::kForward)
            position_ += front_ * velocity;
        if (direction == CameraMovement::kBackward)
            position_ -= front_ * velocity;
        if (direction == CameraMovement::kLeft)
            position_ -= right_ * velocity;
        if (direction == CameraMovement::kRight)
            position_ += right_ * velocity;
    }

    virtual void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true)
    {
        x_offset *= mouse_sensitivity_;
        y_offset *= mouse_sensitivity_;

        yaw_   += x_offset;
        pitch_ += y_offset;

        if (constrain_pitch)
        {
            if (pitch_ > 89.0f)
                pitch_ = 89.0f;
            if (pitch_ < -89.0f)
                pitch_ = -89.0f;
        }

        UpdateCameraVectors();
    }

    void ProcessMouseScroll(float y_offset)
    {
        if (zoom_ >= 1.0f && zoom_ <= 45.0f)
            zoom_ -= y_offset;
        if (zoom_ <= 1.0f)
            zoom_ = 1.0f;
        if (zoom_ >= 45.0f)
            zoom_ = 45.0f;
    }

protected:
    virtual void UpdateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        right_ = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up_    = glm::normalize(glm::cross(right_, front_));
    }
};


class ThirdPersonCamera: public Camera
{

public:

    glm::vec3 main_character_position_;
    GLfloat spring_arm_length_ = 40.f;
    glm::vec3 spring_arm_offset_;

    ThirdPersonCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : Camera(position)
    {
        pitch_ = -20.0f;
        spring_arm_offset_ = glm::vec3(0.0f, -pitch_, spring_arm_length_);
        main_character_position_ = position;
        SetMainCharacterPosition(position);
//        log_dbg("Camera position = %f %f %f", position[0], position[1], position[2]);
        UpdateCameraVectors();
    }


    virtual void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true) override
    {
        x_offset *= mouse_sensitivity_;
        y_offset *= mouse_sensitivity_;


        if (abs(x_offset) > 0.000001 || abs(y_offset) > 0.000001){
            yaw_   += x_offset;
            pitch_ += y_offset;
            GLfloat x = glm::sin(glm::radians(yaw_)) * spring_arm_length_;
            GLfloat y = glm::cos(glm::radians(yaw_)) * spring_arm_length_;
//            log_dbg("Yaw %f Front %f x %f y %f", abs(fmod(glm::radians(Yaw), 2.0)) - 1, Front[0], Front[1], Front[2]);

            if (constrain_pitch)
            {
                if (pitch_ > -0.0f)
                    pitch_ = -0.0f;
                if (pitch_ < -30.0f)
                    pitch_ = -30.0f;
            }

            spring_arm_offset_ = glm::vec3(-y, -pitch_, -x);
            this->position_ = this->main_character_position_ + spring_arm_offset_;
            UpdateCameraVectors();
        }
    }


    void SetMainCharacterPosition(glm::vec3 position){
        this->main_character_position_ = position;
        this->position_ = this->main_character_position_ + spring_arm_offset_;
        UpdateCameraVectors();
    }

    glm::vec3 FrontXZ()
    {
        return glm::vec3(this->front_[0], 0.0f, this->front_[2]);
    }

    glm::vec3 RightXZ()
    {
        return glm::vec3(this->right_[0], 0.0f, this->right_[2]);
    }

protected:

    virtual void UpdateCameraVectors() override
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        right_ = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up_    = glm::normalize(glm::cross(right_, front_));
    }
};
