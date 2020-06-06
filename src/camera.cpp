
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <cmath>
#include "log.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  80.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + glm::vec3(Front[0], Front[1], Front[2]) , Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

protected:
    virtual void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};


class ThirdPersonCamera: public Camera
{

public:


    glm::vec3 mainCharacterPosition;
    GLfloat springArmLength = 40.f;
    glm::vec3 springArmOffset;

    ThirdPersonCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : Camera(position)
    {
        Pitch = -20.0f;
        springArmOffset = glm::vec3(0.0f, -Pitch, springArmLength);
        mainCharacterPosition = position;
        setMainCharacterPosition(position);
//        log_dbg("Camera position = %f %f %f", position[0], position[1], position[2]);
        updateCameraVectors();
    }


    virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;


        if (abs(xoffset) > 0.000001){
            Yaw   += xoffset;
            Pitch += yoffset;
            GLfloat x = glm::sin(glm::radians(Yaw)) * springArmLength;
            GLfloat y = glm::cos(glm::radians(Yaw)) * springArmLength;
//            log_dbg("Yaw %f Front %f x %f y %f", abs(fmod(glm::radians(Yaw), 2.0)) - 1, Front[0], Front[1], Front[2]);

            if (constrainPitch)
            {
                if (Pitch > -10.0f)
                    Pitch = -10.0f;
                if (Pitch < -25.0f)
                    Pitch = -25.0f;
            }

            springArmOffset = glm::vec3(-y, -Pitch, -x);
            this->Position = this->mainCharacterPosition + springArmOffset;
            updateCameraVectors();
        }
    }


    void setMainCharacterPosition(glm::vec3 position){
        this->mainCharacterPosition = position;
        this->Position = this->mainCharacterPosition + springArmOffset;
        updateCameraVectors();
    }


    virtual void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
