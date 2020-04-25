#include "game.h"


Game::Game(GLuint width, GLuint height, Input * input ):
State(GAME_MENU), Width(width), Height (height), inputController(input)
{
}


void Game::Init()
{
    State = GAME_ACTIVE;

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
    this->_prepareGameState();
    this->_updateGameDescription(true);

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

    kobjects[0] = Object(glm::vec2(10, Height / 2 - _platformHeight / 2),
                          glm::vec2(_platformWidth, _platformHeight));

    kobjects[1] = Object(glm::vec2(Width - _platformWidth - 10, Height / 2 - _platformHeight / 2),
                          glm::vec2(_platformWidth, _platformHeight));

    kobjects[2] = Object(glm::vec2(Width / 2 - _ballSide / 2, Height / 2 - _ballSide / 2),
                          glm::vec2(_ballSide, _ballSide));
}

void Game::_prepareGameState()
{
    this->_ballImpulse = 0;
    this->_ballReboundCounter = 0;
    this->_ballSpeed = DEFAULT_BALL_SPEED;
    this->State = GAME_MENU;
}


void Game::_processPlayerWin(const char *playerName)
{
    gameDescription = std::string("Kubarem winner: ") + std::string(playerName) + std::string(". Press SPACE to restart");
    this->_scoreLeft = 0;
    this->_scoreRight = 0;
    this->_prepareGameState();
}


void Game::_updateGameDescription(GLboolean startup)
{
    if (startup)
        gameDescription = std::string("Kubarem pong. Press SPACE to start");
    else
        gameDescription = std::string("Kubarem score: ")
            + std::to_string(_scoreLeft) + "-"
            + std::to_string(_scoreRight);
}


void Game::ProcessInput(GLfloat deltaTime)
{
    if (State == GAME_MENU){
        if (this->inputController->Keys[GLFW_KEY_SPACE])
            {
                this->State = GAME_ACTIVE;
                this->inputController->Keys[GLFW_KEY_SPACE] = GL_FALSE;
                if ((float)glm::linearRand(-0.5, 0.5) > 0)
                    _ballImpulse = 1.0f;
                else
                    _ballImpulse = -1.0f;

                this->_updateGameDescription();
                log_dbg("Game is in active state");
            }
    }
    if (this->State == GAME_ACTIVE)
    {
        // first player controller
        if (this->inputController->Keys[GLFW_KEY_W])
            this->_doRocketBoundsCheck(&kobjects[0], glm::vec2(0.0f, 10.0f));
        if (this->inputController->Keys[GLFW_KEY_S])
            this->_doRocketBoundsCheck(&kobjects[0], glm::vec2(0.0f, -10.0f));
        // second player controller
        if (this->inputController->Keys[GLFW_KEY_UP])
            this->_doRocketBoundsCheck(&kobjects[1], glm::vec2(0.0f, 10.0f));
        if (this->inputController->Keys[GLFW_KEY_DOWN])
            this->_doRocketBoundsCheck(&kobjects[1], glm::vec2(0.0f, -10.0f));
    }
}


void Game::_doRocketBoundsCheck(Object *kobject, glm::vec2 newPosition)
{
    if ( kobject->Position[1] + kobject->Size[1] + newPosition[1] > Height)
        return;
    if ( 0 > kobject->Position[1] + newPosition[1])
        return;
    else
        kobject->Position += newPosition;
}


void Game::_doBallFly(Object *ball)
{
    ball->Position += glm::vec2(this->_ballImpulse * _ballSpeed, this->_ballAngleY * _ballSpeed);

    // check ball rebounds from rocket
    if (ball->Position[0] + _ballSide >= kobjects[1].Position[0])
        if (ball->Position[1] + _ballSide >= kobjects[1].Position[1] and
            ball->Position[1] <= kobjects[1].Position[1] + _platformHeight )
        {
            this->_doBallRebound("Right", -1.0);
            //return to not to check for scores
            return;
        }
    if (ball->Position[0] <= kobjects[0].Position[0])
        if (ball->Position[1] + _ballSide >= kobjects[0].Position[1] and
            ball->Position[1] <= kobjects[0].Position[1] + _platformHeight )
        {
            this->_doBallRebound("Left ", 1.0);
            //return to not to check for scores
            return;
        }

    // check ball rebounds from playground sides
    if ((ball->Position[1] + _ballSide >= this->Height) or
            (ball->Position[1] <= 0))
        this->_ballAngleY *= -1;

    // check scores
    if (ball->Position[0] + _ballSide >= this->Width)
    {
        this->_initObjects();
        this->_prepareGameState();
        this->_scoreLeft++;
        this->_updateGameDescription();
        if (this->_scoreLeft == SCORES_TO_WIN)
            this->_processPlayerWin("Left");

        log_info("GOAL! Left player scored");
    }
    if (ball->Position[0] <= 0)
    {
        this->_initObjects();
        this->_prepareGameState();
        this->_scoreRight++;
        this->_updateGameDescription();
        if (this->_scoreRight == SCORES_TO_WIN)
            this->_processPlayerWin("Right");
        log_info("GOAL! Right player scored");
    }
}

void Game::_doBallRebound(const char *rocketName, float newBallImpulse)
{
    this->_ballImpulse = newBallImpulse;
    this->_ballAngleY = (float)glm::linearRand(-0.7, 0.7);
    this->_ballReboundCounter++;
    if (this->_ballReboundCounter % 5 == 0)
    {
        this->_ballSpeed += 2;
        log_info("Ball speed increased, current speed = %f", _ballSpeed);
    }
    log_dbg("%s player rebound, angle = %f", rocketName, _ballAngleY);
}


void Game::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);
    glBindVertexArray(VAO);

    _doBallFly(&kobjects[2]);
    renderer->drawObject(kobjects[0], kobjects[0].Position, kobjects[0].Size, (GLfloat)0.0);
    renderer->drawObject(kobjects[1], kobjects[1].Position, kobjects[1].Size, (GLfloat)0.0);
    renderer->drawObject(kobjects[2], kobjects[2].Position, kobjects[2].Size, (GLfloat)0.0);
}


void Game::DoCollisions()
{
}
