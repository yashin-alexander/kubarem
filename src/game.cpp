#include "game.h"
#include "log.h"

#include "glm/gtx/string_cast.hpp"


Game::Game(GLuint width, GLuint height, Input * input ):
State(GAME_MENU), Width(width), Height (height), inputController(input)
{
}

void Game::Init()
{
    State = GAME_MENU;
    model = glm::mat4(1.0);
    x_rot = 1.0;

    shaderProgram = loadShaderFromFile("src/shaders/default_vs.glsl", "src/shaders/default_fs.glsl");

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    float vertices[] = {
         40.0f,  40.0f, 0.0f, 0.0f,  // top right
         40.0f, 20.0f, 0.0f, 0.0f,   // bottom right
         20.0f,  20.0f, 0.0f, 0.0f,  // bottom left
         20.0f,  40.0f, 0.0f, 0.0f,  // top left
    };
    unsigned int EBO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->_initProjection();
}


void Game::_initProjection()
{
    glUseProgram(shaderProgram->ID);
    glm::mat4 projection = glm::ortho(0.0f, (float)Width, float(Height), 0.0f, -1.0f, 1.0f );
    this->shaderProgram->SetMatrix4("projection", projection);
}


void Game::ProcessInput(GLfloat deltaTime)
{
    if (State == GAME_MENU){
        if (this->inputController->Keys[GLFW_KEY_TAB])
            {
                this->State = GAME_ACTIVE;
                this->inputController->Keys[GLFW_KEY_TAB] = GL_FALSE;
                log_dbg("Game is in active state");
            }
    }
    if (this->State == GAME_ACTIVE)
    {
        if (this->inputController->Keys[GLFW_KEY_W])
        {
            log_dbg("W is pressed");
            x_rot -= 0.1;
            x_move -= 10.0;
        }
        if (this->inputController->Keys[GLFW_KEY_S])
        {
            log_dbg("S is pressed");
            x_rot += 0.1;
            x_move += 10.0;
        }
        log_dbg("x = %f", x_rot);
    }
}

void Game::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);
    glBindVertexArray(VAO);


    GLint vertexColorLocation = glGetUniformLocation(shaderProgram->ID, "color");
    glUniform4f(vertexColorLocation,
                glm::sin(glfwGetTime()),
                glm::cos(2 * glfwGetTime()),
                glm::cos(x_rot),
                1.0f);


    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,  x_move,  0.0f));
    this->shaderProgram->SetMatrix4("view", view);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f));
    this->shaderProgram->SetMatrix4("model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3( 60.0f,  0.5f,  0.0f));
    this->shaderProgram->SetMatrix4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
