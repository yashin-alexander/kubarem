#include "game.h"


Game::Game(GLuint width, GLuint height, Input * input):
State(GAME_MENU), Width(width), Height (height), inputController(input)
{
}


void Game::Init()
{
    State = GAME_ACTIVE;

    shaderProgram = loadShaderFromFile("src/shaders/default_vs.glsl",
                                       "src/shaders/default_fs.glsl");

    first_planet = new Planet("resources/objects/cyborg/cyborg.obj",
                       shaderProgram,
                       (float)Width / (float)Height,
                       2,
                       -0.587);
}


void Game::_initProjection()
{
    glUseProgram(shaderProgram->ID);
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    this->shaderProgram->SetMatrix4("projection", projection);

    glm::mat4 view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    this->shaderProgram->SetMatrix4("view", projection);
}


void Game::_initObjects()
{

}


void Game::ProcessInput(GLfloat deltaTime)
{
    if (State == GAME_MENU){
        if (this->inputController->Keys[GLFW_KEY_SPACE])
            {
                this->State = GAME_ACTIVE;
                this->inputController->Keys[GLFW_KEY_SPACE] = GL_FALSE;
                log_dbg("Game is in active state");
            }
    }
    if (this->State == GAME_ACTIVE)
    {
        if (this->inputController->Keys[GLFW_KEY_W])
        {
            _cameraPosition += 0.1f;

        }
        if (this->inputController->Keys[GLFW_KEY_S])
        {
            _cameraPosition -= 0.1f;
        }
    }
}


void Game::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);

    first_planet->Render(VAO);
}


void Game::DoCollisions()
{
}
