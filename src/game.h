#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input.h"
#include "shader.h"


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
//    int shaderProgram;
    GLuint VAO;
public:
    // Game state
    GameState              State;
    GLuint                 Width, Height;
    GLuint                 Lives;
    Input                  *inputController;
    Shader 				   *shaderProgram;
    GLuint 					quadVAO;
    glm::mat4 model;
    float x_rot;
    float y_rot;

//    ~Game();
    void Update(GLfloat dt);
//    void Render();
    void DoCollisions();
    void ResetLevel();


    Game(GLuint width, GLuint height, Input * input );

    void Init();
    void ProcessInput(GLfloat deltaTime);
//    void _update_vbo();
    void Render();
};

#endif // GAME_H
