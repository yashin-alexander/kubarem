#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "log.h"
#include "input.h"
#include "object.h"
#include "shader.h"
#include "renderer.h"


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
    void _doRebounds(Kobject *kobject, glm::vec2 newPosition);

public:
    GameState              State;
    GLuint                 Width, Height;
    GLuint                 Lives;
    Input                  *inputController;
    Shader 				   *shaderProgram;

    Renderer			   *renderer = nullptr;
    Kobject  			    kobjects[3];

//    ~Game();
    void Update(GLfloat dt);
//    void Render();
    void DoCollisions();
    void ResetLevel();


    Game(GLuint width, GLuint height, Input * input );

    void Init();
    void ProcessInput(GLfloat deltaTime);
    void InitRenderer();
    void Render();
};

#endif // GAME_H
