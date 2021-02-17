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
#include "scene/scene_serializer.h"
#include "scene/uuid.h"
#include "ai/world_state.h"
#include "ai_actions/ai_action_follow.h"


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 700;


namespace kubarem {
    int GOAPHeuristic(const goap::WorldState &now, const goap::WorldState &goal) {
        return now.distanceTo(goal) * 2;
    }

    class App {
    private:
        GLFWwindow *window;
        kubarem::Scene *scene_;
        glm::vec2 viewport_size;
    public:

        App() {
            this->OpenglSetup();
            this->CreateSceneLayout();
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
            AudioCore::setup_audio_core();
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

            auto *audioPositioned = new AudioPositioned("s.mp3");
            auto *audioBackground = new AudioBackground("s.mp3");
            auto *audioSpeech = new AudioSpeech("", (unsigned int) 530, (float) 10, (float) 0.5, (int) KW_NOISE);

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
            auto tpc_uuid = tpc.getComponent<kubarem::UuidComponent>().uuid;

//            /*
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
            cubeEntity.addComponent<kubarem::AudioPositionedComponent>(audioPositioned);
//            cubeEntity.addComponent<kubarem::PyScriptComponent>("resources.blueprints.cube_blueprint");
            {
                auto cube_uuid = cubeEntity.getComponent<kubarem::UuidComponent>().uuid;
                goap::WorldState cube_goal("Cube goal");
                cube_goal.setFact(tpc_uuid.c_str(), "Dead", true);

                goap::WorldState initial_state("Initial state");
                initial_state.setFact(tpc_uuid.c_str(), "Dead", false);
                initial_state.setFact(cube_uuid.c_str(), "Fuel loaded", false);

                std::vector<goap::Action> actions;

                goap::Action action_follow("Follow tpc", 1);
                action_follow.setPrecondition(tpc_uuid.c_str(), "Dead", false);
                action_follow.setPrecondition(cube_uuid.c_str(), "Fuel loaded", true);
                action_follow.setEffect(tpc_uuid.c_str(), "Dead", true);
                actions.push_back(action_follow);

                goap::Action action_load_fuel("Load fuel", 1);
                action_load_fuel.setPrecondition(cube_uuid.c_str(), "Fuel loaded", false);
                action_load_fuel.setEffect(cube_uuid.c_str(), "Fuel loaded", true);
                actions.push_back(action_load_fuel);

                cubeEntity.addComponent<kubarem::AIComponent>(actions, initial_state, cube_goal, GOAPHeuristic);
            }

            kubarem::Entity floorEntity = scene_->CreateEntity("Floor");
            floorEntity.addComponent<kubarem::CubeObjectComponent>("resources/textures/background.png");
            floorEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, -5, 0), glm::vec3(400, 0.3, 400));
            floorEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");

            kubarem::Entity particlesEmitterEntity = scene_->CreateEntity("ParticleEmitter");
            particlesEmitterEntity.addComponent<kubarem::ParticlesComponent>(particles_parameters, (uint32_t) 1000);
            particlesEmitterEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/particle_vs.glsl");


            kubarem::Entity audio_background = scene_->CreateEntity("AudioBackground");
            audio_background.addComponent<kubarem::AudioBackgroundComponent>(audioBackground);
//             */

//        kubarem::Entity audioSpeech = scene_->CreateEntity("AudioSpeech");
//        audioSpeech.addComponent<kubarem::AudioSpeechComponent>(&soloud_, "You will die! I kill you",
//                                                                (unsigned int) 530, (float) 10, (float) 0.5,
//                                                                (int) KW_NOISE);

//            SceneSerializer serializer(scene_);
//            serializer.Deserialize("scene.yaml");
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
//                this->OnImGuiRender(deltaTime);

                _flush_log();
            }
            this->Close();
        }

        void OnUpdate(float ts) {
            AudioCore::update_3d_audio();
            scene_->OnAIUpdateRuntime(ts);
            scene_->OnUpdateRuntime(ts);
        }

        void OnOpenglRender(float ts) {
            glClearColor(0.3f, 0.f, .0f, 0.1f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            scene_->OnRenderRuntime(ts);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        void Close() {
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

/*
#include "ai/action.h"
#include "ai/planner.h"
#include "ai/world_state.h"

#include <iostream>
#include <vector>

using namespace goap;




int main(void) {
    std::vector<Action> actions;

    std::string gun_uuid = UUID::generate_uuid_v4();
    std::string target_uuid = UUID::generate_uuid_v4();
    std::string car_uuid = UUID::generate_uuid_v4();

    WorldState start("start");
    start.setFact(target_uuid.c_str(), "Dead", false);
    start.setFact(gun_uuid.c_str(), "Loaded", false);
    start.setFact(car_uuid.c_str(), "Open", false);

    WorldState end("end");
    end.setFact(target_uuid.c_str(), "Dead", true);

    goap::Action openCar("OpenCar", 5);
    openCar.setPrecondition(gun_uuid.c_str(), "Loaded", true);
    openCar.setPrecondition(car_uuid.c_str(), "Open", false);
    openCar.setEffect(car_uuid.c_str(), "Open", true);
    actions.push_back(openCar);

    // ------
    goap::Action getGun("GetGun", 4);
    getGun.setPrecondition(car_uuid.c_str(), "Broken", true);
    getGun.setPrecondition(gun_uuid.c_str(), "Loaded", false);
    getGun.setEffect(gun_uuid.c_str(), "Loaded", true);
    actions.push_back(getGun);
    goap::Action breakCar("breakCar", 4);
    breakCar.setPrecondition(car_uuid.c_str(), "Open", false);
    breakCar.setEffect(car_uuid.c_str(), "Broken", true);
    actions.push_back(breakCar);


    goap::Action makeGun("makeGun", 9);
    makeGun.setPrecondition(car_uuid.c_str(), "Open", false);
    makeGun.setPrecondition(gun_uuid.c_str(), "Loaded", false);
    makeGun.setEffect(gun_uuid.c_str(), "Loaded", true);
    actions.push_back(makeGun);

    // -----
    goap::Action shoot("Shoot", 10);
    shoot.setPrecondition(gun_uuid.c_str(), "Loaded", true);
    shoot.setPrecondition(target_uuid.c_str(), "Dead", false);
    shoot.setEffect(target_uuid.c_str(), "Dead", true);
    shoot.setEffect(gun_uuid.c_str(), "Loaded", false);
    actions.push_back(shoot);

    Planner as(GOAPHeuristic);
    try {
        std::vector<Action> the_plan = as.plan(start, end, actions);
        for (std::vector<Action>::reverse_iterator rit = the_plan.rbegin(); rit != the_plan.rend(); ++rit) {
            log_info("%s", rit->name().c_str());
        }
    }
    catch (const std::exception&) {
        std::cout << "Sorry, could not find a path!\n";
    }
}
 */