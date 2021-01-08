#pragma once
#include "game.h"
#include "renderer/framebuffer.h"

namespace kubarem {
    class Editor {
    public:
        Editor();

        virtual ~Editor() = default;

        void OnUpdate(float ts);

        void OnImGuiRender();

        void Run();

        void ImguiSetup();
        void ImguiTeardown();

    private:
        void Close();

        GLFWwindow* window;
        Game *kubarem;
        Framebuffer *framebuffer;
    };
}