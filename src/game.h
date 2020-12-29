#pragma once

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_speech.h>

#include "log.h"
#include "input.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "audio/audio.h"
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

    SoLoud::Soloud soloud_;
    AudioPositioned *sound_file_;
    AudioBackground *background_music_;
    AudioSpeech *speech_phrase_;
//    SoLoud::Wav sample;    // One sample
//    SoLoud::Speech speech;
//    int sound_descriptor;
//    glm::mat4 translator;


    std::string gameDescription = "Kubarem";

    void Update(GLfloat dt);
    void DoCollisions();
    void ResetLevel();


    Game(GLuint width, GLuint height, Input * input);
    ~Game();

    void Init();
    void Shutdown();

    void ProcessInput(GLfloat deltaTime);
    void Render(GLfloat deltaTime);

private:
    void InitProjection();
    void InitObjects();
};
