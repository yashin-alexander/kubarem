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
    GLfloat simulationSpeed = 2.0;

    sun = new Planet("resources/objects/planet/planet.obj",
                     "sun.png",
                     shaderProgram,
                     (float)Width / (float)Height,
                     0,
                     simulationSpeed * 0.2,
                     -0.3f,
                     glm::vec3(1.7f, 1.7f, 1.7f));

    first_planet = new Planet("resources/objects/planet/planet.obj",
                       "mercury.jpg",
                       shaderProgram,
                       (float)Width / (float)Height,
                       10,
                       simulationSpeed * 0.8,
                       1.587,
                       glm::vec3(0.3f, 0.3f, 0.3f));

    second_planet = new Planet("resources/objects/planet/planet.obj",
                       "venus.jpg",
                       shaderProgram,
                       (float)Width / (float)Height,
                       16,
                       simulationSpeed * 0.324,
                       0.587,
                       glm::vec3(0.5f, 0.5f, 0.5f));
    third_planet = new Planet("resources/objects/planet/planet.obj",
                       "earth.jpg",
                       shaderProgram,
                       (float)Width / (float)Height,
                       30,
                       simulationSpeed * 0.2,
                       -0.587,
                       glm::vec3(0.7f, 0.7f, 0.7f));
    moon = new Planet("resources/objects/planet/planet.obj",
                       "moon.jpg",
                       shaderProgram,
                       (float)Width / (float)Height,
                       4,
                       simulationSpeed * 0.8,
                       1.587,
                       glm::vec3(0.2f, 0.2f, 0.2f));
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

    sun->Render(VAO, glm::vec2(0,0) - glm::vec2(sun->size), glm::vec2(0,0));
    first_planet->Render(VAO, sun->position, glm::vec2(sun->size));
    second_planet->Render(VAO, sun->position, glm::vec2(sun->size));
    third_planet->Render(VAO, sun->position, glm::vec2(sun->size));
    moon->Render(VAO, third_planet->position, glm::vec2(third_planet->size));
}



void Game::DoCollisions()
{
}
