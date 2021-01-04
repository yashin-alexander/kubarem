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
#include "scene/scene.h"
#include "scene/entity.h"
#include "scene/components.h"

using namespace kubarem;

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
    GLFWwindow             *window_;

    TextRenderer           *text_renderer_;

    kubarem::Scene *scene_;

    SoLoud::Soloud soloud_;
    AudioPositioned *sound_file_;
    AudioBackground *background_music_;
    AudioSpeech *speech_phrase_;

    std::string gameDescription = "Kubarem";

    void Update(GLfloat dt);

    Game(GLuint width, GLuint height, GLFWwindow* window);
    ~Game();

    void Init();
    void Shutdown();

    void Render(GLfloat deltaTime) const;

private:
};
