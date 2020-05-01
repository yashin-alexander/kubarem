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
#include "object.h"
#include "shader.h"
#include "renderer.h"


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

    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


public:
    GameState              State;
    GLuint                 Width, Height;
    GLuint                 Lives;
    Input                  *inputController;
    Shader 				   *shaderProgram;

    Renderer			   *renderer = nullptr;
    Object  			    kobjects[3];

    std::string gameDescription = "Kubare";

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
