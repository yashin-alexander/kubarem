#include "game.h"


Game::Game(GLuint width, GLuint height, Input * input ):
State(GAME_MENU), Width(width), Height (height), inputController(input)
{
}


void Game::Init()
{
    State = GAME_MENU;

    shaderProgram = loadShaderFromFile("src/shaders/default_vs.glsl",
                                       "src/shaders/default_fs.glsl");

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    float rectangleVertices[] = {
         1.0f,  1.0f,  // top right
         1.0f,  0.0f,  // bottom right
         0.0f,  0.0f,  // bottom left
         0.0f,  1.0f,  // top left
    };
    unsigned int EBO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->_initProjection();
    this->_initObjects();

    renderer = new Renderer(shaderProgram);
}


void Game::_initProjection()
{
    glUseProgram(shaderProgram->ID);
    glm::mat4 projection = glm::ortho(0.0f, (float)Width, 0.0f, float(Height), -1.0f, 1.0f );
    this->shaderProgram->SetMatrix4("projection", projection);
}


void Game::_initObjects()
{
    int _platformHeight = 120;
    int _platformWidth = 10;
    int _ballSide = 20;

    kobjects[0] = Kobject(glm::vec2(5, Height / 2 - _platformHeight / 2),
                          glm::vec2(_platformWidth, _platformHeight));

    kobjects[1] = Kobject(glm::vec2(Width - _platformWidth - 5, Height / 2 - _platformHeight / 2),
                          glm::vec2(_platformWidth, _platformHeight));

    kobjects[2] = Kobject(glm::vec2(Width / 2, Height / 2 - _platformHeight / 2),
                          glm::vec2(_ballSide, _ballSide));
}


void Game::ProcessInput(GLfloat deltaTime)
{
    if (State == GAME_MENU){
        if (this->inputController->Keys[GLFW_KEY_TAB])
            {
                this->State = GAME_ACTIVE;
                this->inputController->Keys[GLFW_KEY_TAB] = GL_FALSE;
                log_dbg("Game is in active state");
            }
    }
    if (this->State == GAME_ACTIVE)
    {
        // first player controller
        if (this->inputController->Keys[GLFW_KEY_W])
        {
            this->_doRebounds(&kobjects[0], glm::vec2(0.0f, 10.0f));
        }
        if (this->inputController->Keys[GLFW_KEY_S])
        {
            this->_doRebounds(&kobjects[0], glm::vec2(0.0f, -10.0f));
        }
        // second player controller
        if (this->inputController->Keys[GLFW_KEY_UP])
        {
            this->_doRebounds(&kobjects[1], glm::vec2(0.0f, 10.0f));
        }
        if (this->inputController->Keys[GLFW_KEY_DOWN])
        {
            this->_doRebounds(&kobjects[1], glm::vec2(0.0f, -10.0f));
        }
    }
}


void Game::_doRebounds(Kobject *kobject, glm::vec2 newPosition)
{
    if ( kobject->Position[1] + kobject->Size[1] + newPosition[1] > Height)
        return;
    if ( 0 > kobject->Position[1] + newPosition[1])
        return;
    else
        kobject->Position += newPosition;
}


void Game::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);
    glBindVertexArray(VAO);

    renderer->drawObject(kobjects[0], kobjects[0].Position, kobjects[0].Size, (GLfloat)0.0);
    renderer->drawObject(kobjects[1], kobjects[1].Position, kobjects[1].Size, (GLfloat)0.0);
    renderer->drawObject(kobjects[2], kobjects[2].Position, kobjects[2].Size, (GLfloat)0.0);

    DoCollisions();
}


void Game::DoCollisions()
{
    // check rebound from sides


}



