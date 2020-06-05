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

    camera = new ThirdPersonCamera();
    mainCharacter = new ThirdPersonCharacter("resources/objects/planet/planet.obj",
                     "sun.png",
                     shaderProgram,
                     (float)Width / (float)Height,
                     glm::vec3(1.7f, 1.7f, 1.7f),
                     camera);

    obj = new Object("resources/objects/planet/planet.obj",
                     "moon.jpg",
                     shaderProgram,
                     (float)Width / (float)Height,
                     glm::vec3(1.7f, 1.7f, 1.7f),
                     glm::vec2(10.0, 0.0f));
    obj1 = new Object("resources/objects/planet/planet.obj",
                     "moon.jpg",
                     shaderProgram,
                     (float)Width / (float)Height,
                     glm::vec3(1.7f, 1.7f, 1.7f),
                     glm::vec2(-10.0, 0.0f));
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
        if (this->inputController->Keys[GLFW_KEY_W]){
//            camera->ProcessKeyboard(FORWARD, deltaTime);
            mainCharacter->ProcessKeyboard(FORWARD, deltaTime);
        }
        if (this->inputController->Keys[GLFW_KEY_S]){
//            camera->ProcessKeyboard(BACKWARD, deltaTime);
            mainCharacter->ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (this->inputController->Keys[GLFW_KEY_A]){
//            camera->ProcessKeyboard(LEFT, deltaTime);
            mainCharacter->ProcessKeyboard(LEFT, deltaTime);
        }
        if (this->inputController->Keys[GLFW_KEY_D]){
//            camera->ProcessKeyboard(RIGHT, deltaTime);
            mainCharacter->ProcessKeyboard(RIGHT, deltaTime);
        }
    }
}


void Game::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);

    if (inputController->MouseOffsetUpdated){
        camera->ProcessMouseMovement(inputController->MouseOffsets[X_OFFSET], inputController->MouseOffsets[Y_OFFSET]);
        inputController->MouseOffsetUpdated = false;
    }
//    log_dbg("%f %f %f", camera->Position[0], camera->Position[1], camera->Position[2]);
//    log_dbg("%f %f %f", camera->Position[0], camera->Position[1], camera->Position[2]);

    mainCharacter->Render(VAO, glm::vec2(0,0) - glm::vec2(mainCharacter->size), glm::vec2(0,0));
    obj->Render(VAO, glm::vec2(0,0) - glm::vec2(mainCharacter->size), glm::vec2(0,0), camera);
    obj1->Render(VAO, glm::vec2(0,0) - glm::vec2(mainCharacter->size), glm::vec2(0,0), camera);
}



void Game::DoCollisions()
{
}
