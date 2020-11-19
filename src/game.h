#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "log.h"
#include "input.h"
#include "model.h"
#include "shader.h"
#include "text_renderer.h"
#include "objects/object.h"
#include "particles/particle_controller.h"

class ThirdPersonCamera;
class ThirdPersonCharacter;

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
    Shader                 *textShaderProgram;
    Shader                 *particleShaderProgram;

    ParticleController     *particleController;
    TextRenderer           *textRenderer;

    ThirdPersonCamera 	   *camera;

    ThirdPersonCharacter *mainCharacter = nullptr;
    ModeledObject *objects[OBJECTS_MAX_AMOUNT];
    ModeledObject *cyborg = nullptr;
    CustomGeometryObject *cube = nullptr;
    CustomGeometryObject *floor = nullptr;

    std::string gameDescription = "Kubarem";

//    ~Game();
    void Update(GLfloat dt);
    void DoCollisions();
    void ResetLevel();


    Game(GLuint width, GLuint height, Input * input);

    void Init();
    void Render(GLfloat deltaTime);
    void ProcessInput(GLfloat deltaTime);
};
