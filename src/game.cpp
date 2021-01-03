#include "game.h"

#include "camera.h"


Game::Game(GLuint width, GLuint height, GLFWwindow* window):
        state_(GameState::kGameMenu), width_(width), height_ (height), window_(window)
{
    scene_ = new kubarem::Scene();
}

Game::~Game()
{
    soloud_.deinit();
}


void Game::Init() {
    state_ = GameState::kGameActive;
    soloud_.init();

    Shader * text_shader_program_ = Shader::LoadFromFile("src/shaders/text_vs.glsl",
                                                         "src/shaders/text_fs.glsl");
    text_renderer_ = new TextRenderer(this->width_, this->height_, text_shader_program_);
    text_renderer_->Load("resources/fonts/default.ttf", 24);

    Shader * main_character_shader_program_ = Shader::LoadFromFile("src/shaders/main_vs.glsl",
                                                          "src/shaders/main_fs.glsl");
    Shader * object_shader_program_ = Shader::LoadFromFile("src/shaders/object_vs.glsl",
                                                  "src/shaders/object_fs.glsl");
    Shader * lamp_shader_program_ = Shader::LoadFromFile("src/shaders/lamp_vs.glsl",
                                                "src/shaders/lamp_fs.glsl");
    Shader * particle_shader_program_ = Shader::LoadFromFile("src/shaders/particle_vs.glsl",
                                                    "src/shaders/particle_fs.glsl");

    ParticleParameters particles_parameters{glm::vec3(45, 0, -300),
                                            glm::vec3(50, 50, 50),
                                            glm::vec4(0.1, 0.1, 0.9, 0.9),
                                            13.8f,
                                            8,
                                            45,
                                            1.0f};
    particle_controller_ = new ParticleController(particles_parameters, 5000, (float) width_ / (float) height_,
                                                  particle_shader_program_);

    // models load
    auto *cyborgModel = new Model("resources/objects/cyborg/cyborg.obj", false);
    auto *sphereModel = new Model("resources/objects/sphere/sphere.obj", false);
    auto *triangleSphereModel = new Model("resources/objects/sphere/triangle/sphere.obj", false);
    auto *thirdPersonCharacterModel = new Model("resources/objects/sphere/disco/sphere.obj", false);

    std::map<std::string, Model> models_map = {
        {std::string("resources/objects/cyborg/cyborg.obj"), *cyborgModel},
        {std::string("resources/objects/sphere/sphere.obj"), *sphereModel},
        {std::string("resources/objects/sphere/triangle/sphere.obj"), *triangleSphereModel},
        {std::string("resources/objects/sphere/disco/sphere.obj"), *thirdPersonCharacterModel}
    };
    std::map<std::string, Shader> shaders_map = {
            {std::string("src/shaders/text_vs.glsl"), *text_shader_program_},
            {std::string("src/shaders/main_vs.glsl"), *main_character_shader_program_},
            {std::string("src/shaders/object_vs.glsl"), *object_shader_program_},
            {std::string("src/shaders/lamp_vs.glsl"), *lamp_shader_program_},
            {std::string("src/shaders/particle_vs.glsl"), *particle_shader_program_}
    };

    /*
    cube_ = new CustomGeometryObject((float) width_ / (float) height_,
                                     object_shader_program_,
                                     camera_,
                                     "resources/textures/minecraft_wood.png",
                                     glm::vec3(30, 6, 30),
                                     glm::vec3(16, 16, 16));
    floor_ = new CustomGeometryObject((float) width_ / (float) height_,
                                      object_shader_program_,
                                      camera_,
                                      "resources/textures/background.png",
                                      glm::vec3(0, -4.5, 0),
                                      glm::vec3(850, 1, 850));
                                      */

    sound_file_ = new AudioPositioned(soloud_, "s.mp3", glm::vec3(0), glm::vec3(0), glm::vec3(0));
    background_music_ = new AudioBackground(soloud_, "s.mp3");
    speech_phrase_ = new AudioSpeech(soloud_, "You will die! I kill you", 530, 10, 0.5, KW_NOISE);
//    sound_file_->RunPlayback();
//    background_music_->RunPlayback();
    speech_phrase_->RunPlayback();

    kubarem::Entity cameraController = scene_->CreateEntity("CameraController");
    cameraController.addComponent<kubarem::CameraComponent>(40.f, 5.f);
    cameraController.addComponent<kubarem::InputComponent>(window_);
    cameraController.addComponent<kubarem::ScreenScaleComponent>((float) width_ / (float) height_);
    cameraController.addComponent<kubarem::ModelsCacheComponent>(models_map);
    cameraController.addComponent<kubarem::ShadersCacheComponent>(shaders_map);

    kubarem::Entity cyborgEntity = scene_->CreateEntity("Cyborg");
    cyborgEntity.addComponent<kubarem::ModelComponent>("resources/objects/cyborg/cyborg.obj");
    cyborgEntity.addComponent<kubarem::IlluminatedComponent>(glm::vec3(0, 0, 0));
    cyborgEntity.addComponent<kubarem::TransformComponent>(glm::vec3(0, 0, -100), glm::vec3(6, 6, 6));
    cyborgEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");

    kubarem::Entity tpc = scene_->CreateEntity("ThirdPersonCharacter");
    tpc.addComponent<kubarem::ThirdPersonCharacterComponent>();
    tpc.addComponent<kubarem::ModelComponent>("resources/objects/sphere/sphere.obj");
    tpc.addComponent<kubarem::IlluminatedComponent>(glm::vec3(0, 0, 0));
    tpc.addComponent<kubarem::TransformComponent>(glm::vec3(0, 0,0), glm::vec3(4));
    tpc.addComponent<kubarem::ShaderProgramComponent>("src/shaders/main_vs.glsl");


    for (int i = 0; i < 525; i++) {
        std::string name = std::string("Ball") + std::to_string(i);
        glm::vec3 position = glm::vec3(cos(i) * 60.0f, cos(2*i) * 10, sin(i) - 20.0f * i);

        kubarem::Entity ballEntity = scene_->CreateEntity(name);
        ballEntity.addComponent<kubarem::ModelComponent>("resources/objects/sphere/sphere.obj");
        ballEntity.addComponent<kubarem::IlluminatedComponent>(glm::vec3(0, 0, 0));
        ballEntity.addComponent<kubarem::TransformComponent>(position, glm::vec3(1));
        ballEntity.addComponent<kubarem::ShaderProgramComponent>("src/shaders/object_vs.glsl");
    }
}

void Game::Shutdown()
{
    delete particle_controller_;
    delete text_renderer_;
    delete sound_file_;
    delete background_music_;
    delete speech_phrase_;
    delete scene_;
}

void Game::Update(GLfloat deltaTime)
{
    sound_file_->UpdatePositioning();
    background_music_->UpdatePositioning();

    soloud_.update3dAudio();
}

void Game::Render(GLfloat deltaTime) const
{
    glClearColor(0.3f, 0.f, .0f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    floor_->Render(main_character_->position);
//    cube_->Render(main_character_->position);

//    particle_controller_->update(deltaTime);
//    particle_controller_->renderParticles(camera_);

/*
    text_renderer_->RenderText(
            std::string("Items collected: 0"),
            glm::vec2(5.0f, 5.0f), 1.0f);
    text_renderer_->RenderText(
            std::string("FPS: ") + std::to_string(int(1 / deltaTime)),
            glm::vec2(this->width_ - 110.f, 5.0f), 1.0f);
*/
    scene_->OnRenderRuntime(deltaTime);
}

