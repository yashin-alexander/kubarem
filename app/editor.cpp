#include "log.h"
#include "utils.h"
#include "editor.h"
#include "utils.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 700;


namespace kubarem {
    Editor::Editor() {
        glfwSetErrorCallback(_glfwErrorCallback);

        auto init_result = glfwInit();
        assert(init_result == GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "kubarem", nullptr, nullptr);
        assert(window != nullptr);
        glfwMakeContextCurrent(window);

//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        kubarem = new Game(SCR_WIDTH, SCR_HEIGHT, window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            log_err("Failed to initialize GLAD");
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(_glMessageCallback, nullptr);

        // Define the viewport dimensions
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        // Setup OpenGL options
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        kubarem->Init();

        FramebufferSpecification spec = FramebufferSpecification();
        spec.Width = 1280;
        spec.Height = 720;
        framebuffer = new Framebuffer(spec);

        this->ImguiSetup();
    }

    void Editor::ImguiSetup() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        const char *glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void Editor::Run() {
        static GLfloat deltaTime = 0.0f;
        static GLfloat lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window)) {
            GLfloat currentFrame = (float) glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            this->OnUpdate(deltaTime);
            this->OnImGuiRender();

            _flush_log();
        }
        this->Close();
    }

    void Editor::OnUpdate(float ts) {
        framebuffer->Bind();
        kubarem->Update(ts);
        kubarem->Render(ts);
        glfwSetWindowTitle(window, kubarem->gameDescription.c_str());
        glfwSwapBuffers(window);
        glfwPollEvents();
        framebuffer->Unbind();
    }

    void Editor::OnImGuiRender() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // -------

        static bool dockingEnabled = true;
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("Exit")) this->Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");

        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", 5);
        ImGui::Text("Quads: %d", 1);
        ImGui::Text("Vertices: %d", 1);
        ImGui::Text("Indices: %d", 1);

        uint32_t textureID = framebuffer->GetColorAttachmentRendererID();

        // for now original texture is mirrored
        // updating uv coordinates in here
        // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#about-texture-coordinates
        ImVec2 uv0 = ImVec2(0.0, 1.0); // upper-left portion of the texture
        ImVec2 uv1 = ImVec2(1.0, 0.0); // bottom-right portion of the texture
        ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{1280, 720}, uv0, uv1);

        ImGui::End();
        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Editor::Close() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        kubarem->Shutdown();

        glfwDestroyWindow(window);
        glfwTerminate();

        log_info("Bye");
    }
}

int main() {
    Editor editor = Editor();
    editor.Run();
    return 0;
}
