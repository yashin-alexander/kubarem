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

const unsigned kMaxObjectsAmount = 100;


enum class GameState {
    kGameActive = 1,
    kGameMenu = 2,
    kGameWin = 3,
    kGameLose = 4,
    kGameStopped = 5
};



class Game
{

public:
    GameState              state_;
    GLuint                 width_, height_;
    GLuint                 lives_;
    Input                  *input_controller_;
    Shader 				   *object_shader_program_;
    Shader 				   *main_character_shader_program_;
    Shader 				   *lamp_shader_program_;
    Shader                 *text_shader_program_;
    Shader                 *particle_shader_program_;

    ParticleController     *particle_controller_;
    TextRenderer           *text_renderer_;

    ThirdPersonCamera 	   *camera_;

    ThirdPersonCharacter   *main_character_ = nullptr;
    ModeledObject          *objects_[kMaxObjectsAmount];
    ModeledObject          *cyborg_ = nullptr;
    CustomGeometryObject   *cube_ = nullptr;
    CustomGeometryObject   *floor_ = nullptr;

    std::string gameDescription = "Kubarem";

    Game(GLuint width, GLuint height, Input * input);

    void Init();
    void Shutdown();

    void DoCollisions();
    void ProcessInput(GLfloat deltaTime);
    void Update(GLfloat dt);
    void Render(GLfloat deltaTime);

private:
    GLuint 	VAO_;
    GLuint  light_VAO_;

    void InitProjection();
    void InitObjects();
};
