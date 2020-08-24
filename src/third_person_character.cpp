#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <stdio.h>
#include "glm/gtx/string_cast.hpp"

#include "log.h"
#include "model.h"
#include "shader.h"
#include "camera.cpp"
#include "objects/object.h"


class MoveRotationKeeper
{
private:
    GLfloat rightDirection = 0.0f;
    GLfloat frontDirection = 0.0f;

    GLfloat frontRotationAmount = 0.0f;
    GLfloat rightRotationAmount = 0.0f;

    GLfloat lastZRotation = 0.0f;
    GLfloat lastXRotation = 0.0f;

    glm::mat4 model;

public:
    MoveRotationKeeper(){}

    void flushDirections()
    {
        rightDirection = 0.0f;
        frontDirection = 0.0f;
    }

    void setMoveRight()
    {
        rightDirection = 1.0f;
        lastXRotation = rightDirection;
    }

    void setMoveLeft()
    {
        rightDirection = -1.0f;
        lastXRotation = rightDirection;
    }

    void setMoveForward()
    {
        frontDirection = 1.0f;
        lastZRotation = frontDirection;
    }

    void setMoveBackward()
    {
        frontDirection = -1.0f;
        lastZRotation = frontDirection;
    }

    void setLastModelMatrix(glm::mat4 model)
    {
        this->model = model;
    }

    glm::mat4 getLastModelMatrix()
    {
        return this->model;
    }

    GLboolean isMovingBothDirections()
    {
        return rightDirection && frontDirection;
    }

    GLfloat getFrontDirection()
    {
        return frontDirection;
    }

    GLfloat getRightDirection()
    {
        return rightDirection;
    }

    GLfloat calculateFrontRotation(GLfloat rotationStep){
        frontRotationAmount += frontDirection * rotationStep;
        return frontRotationAmount;
    }

    GLfloat calculateRightRotation(GLfloat rotationStep){
        rightRotationAmount += rightDirection * rotationStep;
        return rightRotationAmount;
    }
};


class ThirdPersonCharacter
{
private:
    GLfloat _screenScale;
    Model  *_model = nullptr;
    glm::mat4 model;
    Shader *_shaderProgram = nullptr;

    GLfloat speed = 1.0f;
    GLfloat time = 0.0f;

    MoveRotationKeeper * rotationKeeper = nullptr;

public:
    ThirdPersonCamera *_camera = nullptr;
    glm::vec3 position;
    glm::vec3 size;
    ModeledObject * sticked[100];
    GLint objectsSticked = 0;

    ThirdPersonCharacter(const char * modelPath,
           Shader *shader,
           GLfloat screenScale,
           glm::vec3 size,
           ThirdPersonCamera * camera):
      _shaderProgram(shader),
      _screenScale(screenScale),
      size(size),
      _camera(camera)
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        _model = new Model(modelPath, false);

        rotationKeeper = new MoveRotationKeeper();
        rotationKeeper->setLastModelMatrix(
                    glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1.0, 0)));
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
            rotationKeeper->setMoveForward();
        }
        if (direction == BACKWARD){
            newPosition = position - this->_getFront() * velocity;
            rotationKeeper->setMoveBackward();
        }
        if (direction == LEFT){
            newPosition = position - this->_getRight() * velocity;
            rotationKeeper->setMoveLeft();
        }
        if (direction == RIGHT){
            newPosition = position + this->_getRight() * velocity;
            rotationKeeper->setMoveRight();
        }

        this->moveEvent(newPosition);
    }

    void moveEvent(glm::vec3 newPosition)
    {
//        if (isInsidePlate(newPosition)){
            this->position = newPosition;
            this->_camera->setMainCharacterPosition(position);
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

    GLboolean isInsidePlate(glm::vec3 positionToCheck)
    {
        // hardcode plate radius
        return (pow(positionToCheck[0], 2) + pow(positionToCheck[2], 2)) < 120000;
    }

    void Render(GLint VAO, glm::vec2 circling_around, glm::vec2 main_size)
    {
        glUseProgram(_shaderProgram->ID);
        glm::vec3 lightPos(60.f, 20.f, -60.f);
        _shaderProgram->SetVector3f("light.position", lightPos);
        _shaderProgram->SetVector3f("viewPos", _camera->Position);

        // light properties
        GLfloat time = glfwGetTime();
        _shaderProgram->SetVector3f("light.ambient", 1.f, 1.f, 1.f);
        _shaderProgram->SetVector3f("light.diffuse", 0.1f, cos(2*time), sin(time));
        _shaderProgram->SetVector3f("light.specular", 1.0f, .0f, .0f);

        // material properties
        _shaderProgram->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        _shaderProgram->SetFloat("material.shininess", 256.0f);


        glm::mat4 projection = glm::perspective(glm::radians(45.f), _screenScale, 0.1f, 500.0f);
        _shaderProgram->SetMatrix4("projection", projection);

        this->_processRotation();

        glm::mat4 view = _camera->GetViewMatrix();
        view = glm::translate(view, position);
        _shaderProgram->SetMatrix4("view", view);

        glBindVertexArray(VAO);
        _model->Draw(*_shaderProgram);
    }

    void _processRotation(){
        model = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1.0, 0));
        GLfloat rotationDelta = _getRotationMultiplier();

//        /*
        if (rotationKeeper->isMovingBothDirections())
        {
           model = glm::rotate(model,
                               rotationKeeper->getFrontDirection() * rotationKeeper->calculateFrontRotation(rotationDelta),
                               rotationKeeper->getFrontDirection() * _getFront() +
                               rotationKeeper->getRightDirection() * _getRight());
            rotationKeeper->setLastModelMatrix(model);
        } else if (rotationKeeper->getFrontDirection()){
           model = glm::rotate(model,
                               rotationKeeper->calculateFrontRotation(rotationDelta),
                               _getFront());
            rotationKeeper->setLastModelMatrix(model);
        } else if (rotationKeeper->getRightDirection()){
            model = glm::rotate(model,
                                rotationKeeper->calculateRightRotation(rotationDelta),
                                _getRight());
            rotationKeeper->setLastModelMatrix(model);
        } else {
            model = rotationKeeper->getLastModelMatrix();
        }
//        */

        this->_processStickedObjects(model);
        rotationKeeper->flushDirections();

        model = glm::scale(model, size);
        _shaderProgram->SetMatrix4("model", model);
    }

    GLfloat _getRotationMultiplier(){
        if(this->time == 0.0f)
            time = (float)glfwGetTime();

        GLfloat old_time = this->time;
        this->time = (float)glfwGetTime();
        return 9 * (glfwGetTime() - old_time);
    }

    void _processStickedObjects(glm::mat4 model){
        for (int i = 0; i < objectsSticked; i++){
            sticked[i]->SetStickedToPosition(this->position);
            sticked[i]->SetMainObjectRotation(model);
        }
    }

    void doCollisions(ModeledObject * objectsList [], GLint _size){
        ModeledObject *currentObject = nullptr;
        GLfloat distance;

        for (int i = 0; i <= _size; i++)
        {
            currentObject = objectsList[i];
            distance = glm::distance(currentObject->position, position);
            if (!currentObject->isSticked
                and distance <= (size[0] + currentObject->size[0]))
            {
                log_info("New object found!");
                log_info("Distance to object: %f", distance);

                this->sticked[objectsSticked] = currentObject;
                objectsSticked++;
                currentObject->isSticked = true;
                glm::vec3 newObjectPosition  = position - currentObject->position;
//                glm::vec3 newObjectPosition = glm::vec3(4, 0, 0);

                glm::mat4 r = glm::mat4(1);
                r = glm::rotate(r, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
//                r = glm::rotate(r, rotationKeeper->getFrontDirection() * rotationKeeper->calculateFrontRotation(0.0f),
//                                rotationKeeper->getFrontDirection() * _getFront());

                newObjectPosition = glm::vec3(model * glm::vec4(glm::normalize(newObjectPosition), 1.0f));
                log_info("New object position %f %f %f", newObjectPosition[0], newObjectPosition[1], newObjectPosition[2]);
                newObjectPosition = glm::vec3(r * glm::vec4(newObjectPosition, 1.0f));
                log_info("New object position %f %f %f", newObjectPosition[0], newObjectPosition[1], newObjectPosition[2]);
//                newObjectPosition = glm::vec3(model * glm::vec4(glm::normalize(newObjectPosition), 1.0f));

                currentObject->position = newObjectPosition;
            } else {
            }
        }
    }
};

