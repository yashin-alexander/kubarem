#include "editor.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 700;


namespace kubarem {
    Editor::Editor() {
        this->OpenglSetup();
        this->FramebufferSetup();
        this->CreateSceneLayout();
        this->gui_ = new Gui(this->window_, this->framebuffer_, this->scene_);
        this->gui_->ImGuiSetup();
    }

    void Editor::OpenglSetup() {
        glfwSetErrorCallback(_glfwErrorCallback);

        auto init_result = glfwInit();
        assert(init_result == GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kubarem Editor", nullptr, nullptr);
        assert(window_ != nullptr);
        glfwMakeContextCurrent(window_);

        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    }

    void Editor::FramebufferSetup() {
        FramebufferSpecification spec = FramebufferSpecification();
        spec.Width = SCR_WIDTH;
        spec.Height = SCR_HEIGHT;
        framebuffer_ = new Framebuffer(spec);
    }

    void Editor::CreateSceneLayout() {
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
        sceneContext.addComponent<kubarem::CameraComponent>(1.f);
        sceneContext.addComponent<kubarem::InputComponent>(window_);
        sceneContext.addComponent<kubarem::ScreenScaleComponent>((float) SCR_WIDTH / (float) SCR_HEIGHT);
        sceneContext.addComponent<kubarem::ModelsCacheComponent>(models_map);
        sceneContext.addComponent<kubarem::ShadersCacheComponent>(shaders_map);
        auto light_sources = sceneContext.addComponent<kubarem::IlluminateCacheComponent>(std::vector<glm::vec3>({1}));

        kubarem::Entity tpc = scene_->CreateEntity("ThirdPersonCharacter");
        tpc.addComponent<kubarem::ThirdPersonCharacterComponent>(false);
        tpc.addComponent<kubarem::ModelComponent>("resources/objects/sphere/sphere.obj");
        tpc.addComponent<kubarem::TransformComponent>(glm::vec3(1, 2, 3), glm::vec3(4));
        tpc.addComponent<kubarem::ShaderProgramComponent>("src/shaders/main_vs.glsl");

        kubarem::Entity cyborgEntity = scene_->CreateEntity("Cyborg");
        cyborgEntity.addComponent<kubarem::ModelComponent>("resources/objects/cyborg/cyborg.obj");
        cyborgEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, 0, -100), glm::vec3(6, 6, 6));
        cyborgEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");

        for (int i = 0; i < 10; i++) {
            std::string name = std::string("Ball_") + std::to_string(i);
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
//        cubeEntity.addComponent<kubarem::AudioPositionedComponent>(&soloud_, "s.mp3");

        kubarem::Entity floorEntity = scene_->CreateEntity("Floor");
        floorEntity.addComponent<kubarem::CubeObjectComponent>("resources/textures/background.png");
        floorEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, -5, 0), glm::vec3(400, 0.3, 400));
        floorEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");

        kubarem::Entity particlesEmitterEntity = scene_->CreateEntity("ParticleEmitter");
        particlesEmitterEntity.addComponent<kubarem::ParticlesComponent>(particles_parameters, (uint32_t) 1000);
        particlesEmitterEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/particle_vs.glsl");

//        kubarem::Entity audioBackground = scene_->CreateEntity("AudioBackground");
//        audioBackground.addComponent<kubarem::AudioBackgroundComponent>(&soloud_, "s.mp3");

//        kubarem::Entity audioSpeech = scene_->CreateEntity("AudioSpeech");
//        audioSpeech.addComponent<kubarem::AudioSpeechComponent>(&soloud_, "You will die! I kill you",
//                                                                (unsigned int) 530, (float) 10, (float) 0.5,
//                                                                (int) KW_NOISE);
    }

    void Editor::Run() {
        static GLfloat deltaTime = 0.0f;
        static GLfloat lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window_)) {
            GLfloat currentFrame = (float) glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();
            this->OnUpdate(deltaTime);
            this->OnOpenglRender(deltaTime);
            this->gui_->onRender();
            _flush_log();
        }
        this->Close();
    }

    void Editor::OnUpdate(float ts) {
        soloud_.update3dAudio();
        scene_->OnUpdateRuntime(ts);
    }

    void Editor::OnOpenglRender(float ts) {
        framebuffer_->Bind();
        glClearColor(0.3f, 0.f, .0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene_->OnRenderRuntime(ts);
        glfwSwapBuffers(window_);
        framebuffer_->Unbind();
    }

    void Editor::Close() {
        this->gui_->Close();
        delete gui_;
        delete scene_;
        delete framebuffer_;
        glfwDestroyWindow(window_);
        glfwTerminate();
        log_info("Bye.");
    }
}

int main() {
    kubarem::Editor editor = kubarem::Editor();
    editor.Run();
    return 0;
}
