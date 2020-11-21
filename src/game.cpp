#include "game.h"


Game::Game(GLuint width, GLuint height, Input * input):
State(GAME_MENU), Width(width), Height (height), inputController(input)
{
}


void Game::Init()
{
    glGenVertexArrays(1, &VAO);

    State = GAME_ACTIVE;

    textShaderProgram = loadShaderFromFile("src/shaders/text_vs.glsl",
                                           "src/shaders/text_fs.glsl");
    textRenderer = new TextRenderer(this->Width, this->Height, textShaderProgram);
    textRenderer->Load("resources/fonts/default.ttf", 24);

    mainCharacterShaderProgram = loadShaderFromFile("src/shaders/main_vs.glsl",
                                                    "src/shaders/main_fs.glsl");

    objectShaderProgram = loadShaderFromFile("src/shaders/object_vs.glsl",
                                             "src/shaders/object_fs.glsl");

    lampShaderProgram = loadShaderFromFile("src/shaders/lamp_vs.glsl",
                                           "src/shaders/lamp_fs.glsl");
    particleShaderProgram = loadShaderFromFile("src/shaders/particle_vs.glsl",
                                               "src/shaders/particle_fs.glsl");


    ParticleParameters particles_parameters{glm::vec3(15, 0, -75),
                                             glm::vec3(50, 50, 50),
                                             13.8f,
                                             5,
                                             45,
                                             1.0f};
    particleController = new ParticleController(particles_parameters,  5000, (float)Width / (float)Height, particleShaderProgram);

    camera = new ThirdPersonCamera();

    // models load
    Model * cyborgModel = new Model("resources/objects/cyborg/cyborg.obj", false);
    Model * sunModel = new Model("resources/objects/sphere/sphere.obj", false);
    Model * triangleSphereModel = new Model("resources/objects/sphere/triangle/sphere.obj", false);

    mainCharacter = new ThirdPersonCharacter("resources/objects/sphere/disco/sphere.obj",
                     mainCharacterShaderProgram,
                     (float)Width / (float)Height,
                     glm::vec3(4.f, 4.f, 4.f),
                     camera);

    cube = new CustomGeometryObject((float)Width / (float)Height,
                                    objectShaderProgram,
                                    camera,
                                    &mainCharacter->position,
                                    "resources/textures/minecraft_wood.png",
                                    glm::vec3(0, 6, 0),
                                    glm::vec3(16, 16, 16));
    floor = new CustomGeometryObject((float)Width / (float)Height,
                                     objectShaderProgram,
                                     camera,
                                     &mainCharacter->position,
                                     "resources/textures/background.png",
                                     glm::vec3(0, -4.5, 0),
                                     glm::vec3(850, 1, 850));

    cyborg = new ModeledObject((float)Width / (float)Height,
                               objectShaderProgram,
                               cyborgModel,
                               camera,
                               &mainCharacter->position,
                               glm::vec3(4.7f, 6.0f, 4.7f),
                               glm::vec3(4.7f, 4.7f, 4.7f));

    for (int i = 0; i < 15; i++){
        objects[i] = new ModeledObject((float)Width / (float)Height,
                                       objectShaderProgram,
                                       triangleSphereModel,
                                       camera,
                                       &mainCharacter->position,
                                       glm::vec3(cos(i) * 60.0f, cos(i) * 2, sin(i) - 10.0f * i),
                                       glm::vec3(2, 2, 2));
        log_info("Object %d created", i);
    }
    for (int i = 15; i < 25; i++){
        objects[i] = new ModeledObject((float)Width / (float)Height,
                                       objectShaderProgram,
                                       sunModel,
                                       camera,
                                       &mainCharacter->position,
                                       glm::vec3(cos(i) * 60.0f, cos(i) * 2, sin(i) - 10.0f * i),
                                       glm::vec3(2, 2, 2));
        log_info("Object %d created", i);
    }
}


void Game::_initProjection()
{
    glUseProgram(objectShaderProgram->ID);
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    this->objectShaderProgram->SetMatrix4("projection", projection);

    glm::mat4 view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    this->objectShaderProgram->SetMatrix4("view", projection);
}


void Game::_initObjects()
{

}


void Game::ProcessInput(GLfloat deltaTime)
{
    if (State == GAME_MENU){
        if (this->inputController->Keys[GLFW_KEY_SPACE])
            {
                this->State = GAME_ACTIVE;
                this->inputController->Keys[GLFW_KEY_SPACE] = GL_FALSE;
                log_dbg("Game is in active state");
            }
    }
    if (this->State == GAME_ACTIVE)
    {
        if (this->inputController->Keys[GLFW_KEY_W]){
            mainCharacter->ProcessKeyboard(FORWARD, deltaTime);
        }
        if (this->inputController->Keys[GLFW_KEY_S]){
            mainCharacter->ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (this->inputController->Keys[GLFW_KEY_A]){
            mainCharacter->ProcessKeyboard(LEFT, deltaTime);
        }
        if (this->inputController->Keys[GLFW_KEY_D]){
            mainCharacter->ProcessKeyboard(RIGHT, deltaTime);
        }
    }
}


void Game::Render(GLfloat deltaTime)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (inputController->MouseOffsetUpdated){
        camera->ProcessMouseMovement(inputController->MouseOffsets[X_OFFSET], inputController->MouseOffsets[Y_OFFSET]);
        inputController->MouseOffsetUpdated = false;
    }
    floor->Render();
    cube->Render();
    cyborg->Render();

    mainCharacter->Render(VAO, glm::vec2(0,0) - glm::vec2(mainCharacter->size), glm::vec2(0,0));

    for (int i = 0; i < 24; i++){
        objects[i]->Render();
    }

    particleController->update(deltaTime);
    particleController->renderParticles(camera);

    textRenderer->RenderText(
                std::string("Items collected: ") + std::to_string(mainCharacter->objectsSticked),
                glm::vec2(5.0f, 5.0f), 1.0f);
    textRenderer->RenderText(
                std::string("FPS: ") + std::to_string(int(1 / deltaTime)),
                glm::vec2(this->Width - 110.f, 5.0f), 1.0f);
}



void Game::DoCollisions()
{
    mainCharacter->doCollisions(objects, 24);
}
