#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/random.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "log.h"
#include "box.h"
#include "input.h"
#include "model.h"
#include "shader.h"
#include "object.h"
#include "camera.cpp"
#include "third_person_character.cpp"


#define OBJECTS_MAX_AMOUNT 100


enum GameState {
    GAME_ACTIVE = 1,
    GAME_MENU = 2,
    GAME_WIN = 3,
    GAME_LOSE = 4,
    GAME_STOPPED =5
};



class Game
{
private:
    GLuint 	VAO;
    GLuint  LightVAO;

    void _initProjection();
    void _initObjects();

public:
    GameState              State;
    GLuint                 Width, Height;
    GLuint                 Lives;
    Input                  *inputController;
    Shader 				   *objectShaderProgram;
    Shader 				   *mainCharacterShaderProgram;
    Shader 				   *lampShaderProgram;

    ThirdPersonCamera 	   *camera;

    ThirdPersonCharacter *mainCharacter = nullptr;
    Object *objects[OBJECTS_MAX_AMOUNT];
    Object *cyborg = nullptr;
    Box *box = nullptr;
    Box *box1 = nullptr;
    Box *lamp = nullptr;

    std::string gameDescription = "Kubarem";

//    ~Game();
    void Update(GLfloat dt);
    void DoCollisions();
    void ResetLevel();


    Game(GLuint width, GLuint height, Input * input);

    void Init();
    void Render();
    void ProcessInput(GLfloat deltaTime);
};

#endif // GAME_H
