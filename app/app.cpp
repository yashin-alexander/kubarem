#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "input.h"
#include "model.h"
#include "shader.h"
#include "audio/audio.h"
#include "scene/scene.h"
#include "scene/entity.h"
#include "text_renderer.h"
#include "scene/components.h"
#include "renderer/framebuffer.h"
#include "particles/particle_controller.h"


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 700;


namespace kubarem {
    class App {
    private:
        GLFWwindow *window;
        kubarem::Scene *scene_;
        glm::vec2 viewport_size;
        SoLoud::Soloud soloud_;
    public:

        App() {
            this->OpenglSetup();
            this->CreateSceneLayout();
            this->ImGuiSetup();
        }

        void OpenglSetup() {
            glfwSetErrorCallback(_glfwErrorCallback);

            auto init_result = glfwInit();
            assert(init_result == GLFW_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kubarem Editor", nullptr, nullptr);
            assert(window != nullptr);
            glfwMakeContextCurrent(window);

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

            FramebufferSpecification spec = FramebufferSpecification();
            spec.Width = SCR_WIDTH;
            spec.Height = SCR_HEIGHT;
        }

        void CreateSceneLayout() {
            soloud_.init();
            scene_ = new kubarem::Scene();

            Shader *text_shader_program_ = Shader::LoadFromFile("src/shaders/text_vs.glsl", "src/shaders/text_fs.glsl");
            Shader *main_character_shader_program_ = Shader::LoadFromFile("src/shaders/main_vs.glsl",
                                                                          "src/shaders/main_fs.glsl");
            Shader *object_shader_program_ = Shader::LoadFromFile("src/shaders/object_vs.glsl",
                                                                  "src/shaders/object_fs.glsl");
            Shader *lamp_shader_program_ = Shader::LoadFromFile("src/shaders/lamp_vs.glsl", "src/shaders/lamp_fs.glsl");
            Shader *particle_shader_program_ = Shader::LoadFromFile("src/shaders/particle_vs.glsl",
                                                                    "src/shaders/particle_fs.glsl");

            ParticleParameters particles_parameters{glm::vec3(45, 0, -300),
                                                    glm::vec3(50, 50, 50),
                                                    glm::vec4(0.1, 0.1, 0.9, 0.9),
                                                    13.8f,
                                                    8,
                                                    45,
                                                    1.0f};

            // models load
            auto *cyborgModel = new Model("resources/objects/cyborg/cyborg.obj", false);
            auto *sphereModel = new Model("resources/objects/sphere/sphere.obj", false);
            auto *triangleSphereModel = new Model("resources/objects/sphere/triangle/sphere.obj", false);
            auto *thirdPersonCharacterModel = new Model("resources/objects/sphere/disco/sphere.obj", false);

            std::map<std::string, Model> models_map = {
                    {std::string("resources/objects/cyborg/cyborg.obj"),          *cyborgModel},
                    {std::string("resources/objects/sphere/sphere.obj"),          *sphereModel},
                    {std::string("resources/objects/sphere/triangle/sphere.obj"), *triangleSphereModel},
                    {std::string("resources/objects/sphere/disco/sphere.obj"),    *thirdPersonCharacterModel}
            };
            std::map<std::string, Shader> shaders_map = {
                    {std::string("src/shaders/text_vs.glsl"),     *text_shader_program_},
                    {std::string("src/shaders/main_vs.glsl"),     *main_character_shader_program_},
                    {std::string("src/shaders/object_vs.glsl"),   *object_shader_program_},
                    {std::string("src/shaders/lamp_vs.glsl"),     *lamp_shader_program_},
                    {std::string("src/shaders/particle_vs.glsl"), *particle_shader_program_},
            };

            kubarem::Entity sceneContext = scene_->CreateEntity("SceneContext");
            sceneContext.addComponent<kubarem::CameraComponent>(40.f, 1.f);
            sceneContext.addComponent<kubarem::InputComponent>(window);
            sceneContext.addComponent<kubarem::ScreenScaleComponent>((float) SCR_WIDTH / (float) SCR_HEIGHT);
            sceneContext.addComponent<kubarem::ModelsCacheComponent>(models_map);
            sceneContext.addComponent<kubarem::ShadersCacheComponent>(shaders_map);
            auto light_sources = sceneContext.addComponent<kubarem::IlluminateCacheComponent>(
                    std::vector<glm::vec3>({1}));

            kubarem::Entity tpc = scene_->CreateEntity("ThirdPersonCharacter");
            tpc.addComponent<kubarem::ThirdPersonCharacterComponent>(true);
            tpc.addComponent<kubarem::ModelComponent>("resources/objects/sphere/sphere.obj");
            tpc.addComponent<kubarem::TransformComponent>(glm::vec3(1, 2, 3), glm::vec3(4));
            tpc.addComponent<kubarem::ShaderProgramComponent>("src/shaders/main_vs.glsl");

            kubarem::Entity cyborgEntity = scene_->CreateEntity("Cyborg");
            cyborgEntity.addComponent<kubarem::ModelComponent>("resources/objects/cyborg/cyborg.obj");
            cyborgEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, 0, -100), glm::vec3(6, 6, 6));
            cyborgEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");

            for (int i = 0; i < 525; i++) {
                std::string name = std::string("Ball") + std::to_string(i);
                glm::vec3 position = glm::vec3(cos(i) * 60.0f, cos(2 * i) * 10, sin(i) - 20.0f * i);

                kubarem::Entity ballEntity = scene_->CreateEntity(name);
                ballEntity.addComponent<kubarem::ModelComponent>("resources/objects/sphere/sphere.obj");
                ballEntity.addComponent<kubarem::TransformComponent>(position, glm::vec3(1));
                ballEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");
            }

            kubarem::Entity cubeEntity = scene_->CreateEntity("Cube");
            cubeEntity.addComponent<kubarem::CubeObjectComponent>("resources/textures/minecraft_wood.png");
            cubeEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, 0, -40), glm::vec3(10));
            cubeEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");
            cubeEntity.addComponent<kubarem::AudioPositionedComponent>(&soloud_, "s.mp3");

            kubarem::Entity floorEntity = scene_->CreateEntity("Floor");
            floorEntity.addComponent<kubarem::CubeObjectComponent>("resources/textures/background.png");
            floorEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, -5, 0), glm::vec3(400, 0.3, 400));
            floorEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");

            kubarem::Entity particlesEmitterEntity = scene_->CreateEntity("ParticleEmitter");
            particlesEmitterEntity.addComponent<kubarem::ParticlesComponent>(particles_parameters, (uint32_t) 1000);
            particlesEmitterEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/particle_vs.glsl");


        kubarem::Entity audioBackground = scene_->CreateEntity("AudioBackground");
        audioBackground.addComponent<kubarem::AudioBackgroundComponent>(&soloud_, "s.mp3");

//        kubarem::Entity audioSpeech = scene_->CreateEntity("AudioSpeech");
//        audioSpeech.addComponent<kubarem::AudioSpeechComponent>(&soloud_, "You will die! I kill you",
//                                                                (unsigned int) 530, (float) 10, (float) 0.5,
//                                                                (int) KW_NOISE);
        }


        void ImGuiSetup() {
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

        void Run() {
            static GLfloat deltaTime = 0.0f;
            static GLfloat lastFrame = 0.0f;

            while (!glfwWindowShouldClose(window)) {
                GLfloat currentFrame = (float) glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                this->OnUpdate(deltaTime);
                this->OnOpenglRender(deltaTime);
                this->OnImGuiRender(deltaTime);

                _flush_log();
            }
            this->Close();
        }

        void OnUpdate(float ts) {
            soloud_.update3dAudio();
            scene_->OnUpdateRuntime(ts);
        }

        void OnOpenglRender(float ts) {
            glClearColor(0.3f, 0.f, .0f, 0.1f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            scene_->OnRenderRuntime(ts);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        void OnImGuiRender(float ts) {
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

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::PopStyleVar();
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

        void Close() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            delete scene_;
            glfwDestroyWindow(window);
            glfwTerminate();
            log_info("Bye");
        }
    };
}

int main() {
    kubarem::App app = kubarem::App();
    app.Run();
    return 0;
}

