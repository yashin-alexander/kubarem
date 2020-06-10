#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <stdio.h>
#include "glm/gtx/string_cast.hpp"

#include "log.h"
#include "model.h"
#include "shader.h"
#include "camera.cpp"


class ThirdPersonCharacter
{
private:
    GLfloat _screenScale;
    Shader *_shaderProgram = nullptr;
    Model  *_model = nullptr;
    glm::vec3 _right;
    ThirdPersonCamera *_camera = nullptr;
    GLfloat frontRotator = 0;
    GLfloat rightRotator = 0;

    GLint invertedConrolFrames = 0.0f;
    GLfloat speed = 1.0f;

public:
    glm::vec3 position;
    glm::vec3 size;

    ThirdPersonCharacter(const char * modelPath,
           const char * texturePath,
           Shader *shader,
           GLfloat screenScale,
           glm::vec3 planetSize,
           ThirdPersonCamera * camera):
      _shaderProgram(shader),
      _screenScale(screenScale),
      size(planetSize),
      _camera(camera)
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        _model = new Model(modelPath, false, texturePath);
//        _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    glm::vec3 _getFront(){
        return glm::vec3(_camera->Front[0],
                         0.0f,
                         _camera->Front[2]);
    }

    glm::vec3 _getRight(){
        return _camera->Right;
    }


    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = deltaTime * speed * 120;
        glm::vec3 newPosition;

        if (direction == FORWARD){
            newPosition = position + this->_getFront() * velocity;
            frontRotator = 1.0;
        }
        if (direction == BACKWARD){
            newPosition = position - this->_getFront() * velocity;
            frontRotator = -1.0;
        }
        if (direction == LEFT){
            newPosition = position - this->_getRight() * velocity;
            rightRotator = -1.0f;
        }
        if (direction == RIGHT){
            newPosition = position + this->_getRight() * velocity;
            rightRotator = 1.0f;
        }

        this->moveEvent(newPosition);
    }

    void moveEvent(glm::vec3 newPosition)
    {
        if (isInsidePlate(newPosition)){
            this->position = newPosition;
            this->_camera->setMainCharacterPosition(position);
        } else {
            if (abs(speed) < 8){
                speed *= -2.0f;
            }
            else {
                speed /= -1.0f;
            }
            log_info("Bounds touched! Controls inverted!");
        }
    }

    GLboolean isInsidePlate(glm::vec3 positionToCheck)
    {
        // hardcode plate radius
        return (pow(positionToCheck[0], 2) + pow(positionToCheck[2], 2)) < 120000;
    }

    void Render(GLint VAO, glm::vec2 circling_around, glm::vec2 main_size)
    {

        glm::mat4 projection = glm::perspective(glm::radians(45.f), _screenScale, 0.1f, 500.0f);
        _shaderProgram->SetMatrix4("projection", projection);

        this->_processRotation();

        glm::mat4 view = _camera->GetViewMatrix();
        view = glm::translate(view, position);
        log_dbg("Position %f %f %f", position[0], position[1], position[2]);
        _shaderProgram->SetMatrix4("view", view);

        glBindVertexArray(VAO);
        _model->Draw(*_shaderProgram);
    }

    void _processRotation(){
        glm::mat4 model = glm::mat4(1.0f);
        GLfloat time = (float)glfwGetTime();
        GLfloat rotationMutiplier = time * 3;

        model = glm::rotate(model, glm::radians(_camera->Yaw), glm::vec3(0, -1.0, 0));
        if (rightRotator and frontRotator){
            model = glm::rotate(model, 0.75f * -frontRotator * rightRotator, glm::vec3(0.0, 1.0, 0.0));
            model = glm::rotate(model, rotationMutiplier, glm::vec3(0.0f, 0.0f, -frontRotator));
        } else if (rightRotator)
            model = glm::rotate(model, rotationMutiplier, glm::vec3(rightRotator, 0.0f, 0.0f));
        else if (frontRotator)
            model = glm::rotate(model, rotationMutiplier, glm::vec3(0.0f, 0.0f, -frontRotator));

        model = glm::scale(model, size);
        _shaderProgram->SetMatrix4("model", model);

        _flushRotators();
    }

    void _flushRotators(){
        frontRotator = 0.0f;
        rightRotator = 0.0f;
    }

};
