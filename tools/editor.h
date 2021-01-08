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
#include "scene/scene.h"
#include "scene/entity.h"
#include "text_renderer.h"
#include "objects/object.h"
#include "scene/components.h"
#include "renderer/framebuffer.h"
#include "particles/particle_controller.h"


namespace kubarem {
    class Editor {
    public:
        Editor();
        virtual ~Editor() = default;

        void OpenglSetup();
        void CreateSceneLayout();
        void ImGuiSetup();

        void Run();
        void OnUpdate(float ts);
        void OnImGuiRender(float ts);
        void OnOpenglRender(float ts);

    private:
        void Close();

        GLFWwindow* window;
        kubarem::Scene *scene_;
        Framebuffer *framebuffer;
        glm::vec2 viewport_size;
        SoLoud::Soloud soloud_;
    };
}