#include "game.h"

#include "camera.h"


Game::Game(GLuint width, GLuint height, Input * input):
        state_(GameState::kGameMenu), width_(width), height_ (height), input_controller_(input)
{
}

Game::~Game()
{
    soloud_.deinit();
}


void Game::Init() {
    state_ = GameState::kGameActive;
    soloud_.init();

    text_shader_program_ = Shader::LoadFromFile("src/shaders/text_vs.glsl",
                                                "src/shaders/text_fs.glsl");
    text_renderer_ = new TextRenderer(this->width_, this->height_, text_shader_program_);
    text_renderer_->Load("resources/fonts/default.ttf", 24);

    main_character_shader_program_ = Shader::LoadFromFile("src/shaders/main_vs.glsl",
                                                          "src/shaders/main_fs.glsl");

    object_shader_program_ = Shader::LoadFromFile("src/shaders/object_vs.glsl",
                                                  "src/shaders/object_fs.glsl");

    lamp_shader_program_ = Shader::LoadFromFile("src/shaders/lamp_vs.glsl",
                                                "src/shaders/lamp_fs.glsl");
    particle_shader_program_ = Shader::LoadFromFile("src/shaders/particle_vs.glsl",
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

    camera_ = new ThirdPersonCamera();

    // models load
    Model *cyborgModel = new Model("resources/objects/cyborg/cyborg.obj", false);
    Model *sunModel = new Model("resources/objects/sphere/sphere.obj", false);
    Model *triangleSphereModel = new Model("resources/objects/sphere/triangle/sphere.obj", false);
    Model *thirdPersonCharacterModel = new Model("resources/objects/sphere/disco/sphere.obj", false);

    main_character_ = new ThirdPersonCharacter((float) width_ / (float) height_,
                                               main_character_shader_program_,
                                               thirdPersonCharacterModel,
                                               camera_,
                                               glm::vec3(4.0f));

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

    cyborg_ = new ModeledObject((float) width_ / (float) height_,
                                object_shader_program_,
                                cyborgModel,
                                camera_,
                                glm::vec3(0, -2, 50),
                                glm::vec3(4.7f, 4.7f, 4.7f));

    for (int i = 0; i < 15; i++) {
        objects_[i] = new ModeledObject((float) width_ / (float) height_,
                                        object_shader_program_,
                                        triangleSphereModel,
                                        camera_,
                                        glm::vec3(cos(i) * 60.0f, cos(i) * 2, sin(i) - 10.0f * i),
                                        glm::vec3(2, 2, 2));
        log_info("Object %d created", i);
    }
    for (int i = 15; i < 25; i++) {
        objects_[i] = new ModeledObject((float) width_ / (float) height_,
                                        object_shader_program_,
                                        sunModel,
                                        camera_,
                                        glm::vec3(cos(i) * 60.0f, cos(i) * 2, sin(i) - 10.0f * i),
                                        glm::vec3(2, 2, 2));
        log_info("Object %d created", i);
    }
    sound_file_ = new AudioPositioned(soloud_, "s.mp3", cube_->position, camera_->position_, camera_->front_);
    background_music_ = new AudioBackground(soloud_, "s.mp3");
    speech_phrase_ = new AudioSpeech(soloud_, "You will die! I kill you", 530, 10, 0.5, KW_NOISE);
//    sound_file_->RunPlayback();
//    background_music_->RunPlayback();
    speech_phrase_->RunPlayback();
}

void Game::Shutdown()
{
    for(int i = 0; i < 25; ++i)
    {
        delete objects_[i];
    }
    delete cyborg_;
    delete floor_;
    delete cube_;
    delete main_character_;
    delete camera_;
    delete particle_controller_;
    delete text_renderer_;
    delete sound_file_;
    delete background_music_;
    delete speech_phrase_;
}

void Game::DoCollisions()
{
}

void Game::ProcessInput(GLfloat deltaTime)
{
    if (state_ == GameState::kGameMenu){
        if (this->input_controller_->Keys[GLFW_KEY_SPACE])
        {
            this->state_ = GameState::kGameActive;
            this->input_controller_->Keys[GLFW_KEY_SPACE] = GL_FALSE;
            log_dbg("Game is in active state");
        }
    }
    if (this->state_ == GameState::kGameActive)
    {
        if (this->input_controller_->Keys[GLFW_KEY_W]){
            main_character_->ProcessKeyboard(CameraMovement::kForward, deltaTime);
        }
        if (this->input_controller_->Keys[GLFW_KEY_S]){
            main_character_->ProcessKeyboard(CameraMovement::kBackward, deltaTime);
        }
        if (this->input_controller_->Keys[GLFW_KEY_A]){
            main_character_->ProcessKeyboard(CameraMovement::kLeft, deltaTime);
        }
        if (this->input_controller_->Keys[GLFW_KEY_D]){
            main_character_->ProcessKeyboard(CameraMovement::kRight, deltaTime);
        }
    }
}

void Game::Update(GLfloat deltaTime)
{
    sound_file_->UpdatePositioning();
    background_music_->UpdatePositioning();

    soloud_.update3dAudio();
}

void Game::Render(GLfloat deltaTime)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (input_controller_->MouseOffsetUpdated){
        camera_->ProcessMouseMovement(input_controller_->MouseOffsets[X_OFFSET], input_controller_->MouseOffsets[Y_OFFSET]);
        input_controller_->MouseOffsetUpdated = false;
    }

    floor_->Render(main_character_->position);
    cube_->Render(main_character_->position);
    cyborg_->Render(main_character_->position);

    main_character_->Render(main_character_->position);

    for (int i = 0; i < 25; i++) {
        objects_[i]->Render(main_character_->position);
    }

    for (int i = 0; i < 24; i++){
        objects_[i]->Render(main_character_->position);
    }

    particle_controller_->update(deltaTime);
    particle_controller_->renderParticles(camera_);

    text_renderer_->RenderText(
            std::string("Items collected: 0"),
            glm::vec2(5.0f, 5.0f), 1.0f);
    text_renderer_->RenderText(
            std::string("FPS: ") + std::to_string(int(1 / deltaTime)),
            glm::vec2(this->width_ - 110.f, 5.0f), 1.0f);
}

void Game::InitProjection()
{
    glUseProgram(object_shader_program_->program_ID_);
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width_ / (GLfloat)height_, 0.1f, 100.0f);
    this->object_shader_program_->SetMatrix4("projection", projection);

    glm::mat4 view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
    this->object_shader_program_->SetMatrix4("view", projection);
}


void Game::InitObjects()
{

}
