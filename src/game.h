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
#include "input.h"
#include "model.h"
#include "shader.h"
#include "object.cpp"
#include "map.cpp"
#include "camera.cpp"
#include "third_person_character.cpp"


#define DEFAULT_BALL_SPEED 10.0
#define SCORES_TO_WIN 5


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
    GLuint 					VAO;

    void _initProjection();
    void _initObjects();



public:
    GameState              State;
    GLuint                 Width, Height;
    GLuint                 Lives;
    Input                  *inputController;
    Shader 				   *shaderProgram;

    ThirdPersonCamera 	   *camera;

    ThirdPersonCharacter *mainCharacter = nullptr;
    Object *obj = nullptr;
    Object *obj1 = nullptr;
    Object *obj2 = nullptr;
    Map *map = nullptr;

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
