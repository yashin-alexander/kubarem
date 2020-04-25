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
    void _prepareGameState();
    void _doRocketBoundsCheck(Object *kobject, glm::vec2 newPosition);
    void _doBallRebound(const char * rocketName, float newBallImpulse);
    void _doBallFly(Object *ball);
    void _updateGameDescription(GLboolean startup = false);
    void _processPlayerWin(const char * playerName);

    int _platformHeight = 120;
    int _platformWidth = 10;
    int _ballSide = 20;

public:
    GameState              State;
    GLuint                 Width, Height;
    GLuint                 Lives;
    Input                  *inputController;
    Shader 				   *shaderProgram;

    Renderer			   *renderer = nullptr;
    Object  			    kobjects[3];


    GLfloat 	_ballSpeed = DEFAULT_BALL_SPEED;
    GLfloat     _ballImpulse = 1.0; // ball goes right
    GLfloat     _ballAngleY = 0.0;  // ball movement angle, ball moves straight
    GLint		_ballReboundCounter = 0;
    GLint       _scoreLeft = 0;
    GLint       _scoreRight = 0;
    std::string gameDescription = "Kubarem 0-0";

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
