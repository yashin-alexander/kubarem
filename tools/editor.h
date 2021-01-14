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
#include <pybind11/embed.h>

#include "log.h"
#include "input.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "audio/audio.h"
#include "scene/scene.h"
#include "scene/entity.h"
#include "text_renderer.h"
#include "objects/object.h"
#include "scene/components.h"
#include "renderer/framebuffer.h"
#include "particles/particle_controller.h"
#include "gui.h"


namespace kubarem {
    class Editor {
    public:
        Editor();
        virtual ~Editor() = default;

        void OpenglSetup();
        void CreateSceneLayout();

        void Run();
        void OnUpdate(float ts);
        void OnOpenglRender(float ts);
    private:

        void FramebufferSetup();
        void Close();

        GLFWwindow* window_;
        kubarem::Scene *scene_;
        Framebuffer *framebuffer_;
        Gui *gui_;
    };
}