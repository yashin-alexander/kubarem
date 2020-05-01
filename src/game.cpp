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

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);

//    this->_initProjection();
//    this->_initObjects();

//    renderer = new Renderer(shaderProgram);
}


void Game::_initProjection()
{
    glUseProgram(shaderProgram->ID);
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    this->shaderProgram->SetMatrix4("projection", projection);

    glm::mat4 view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    this->shaderProgram->SetMatrix4("view", projection);
}


void Game::_initObjects()
{

//    kobjects[0] = Object(glm::vec2(Width / 2 - _ballSide / 2, Height / 2 - _ballSide / 2),
//                          glm::vec2(_ballSide, _ballSide));
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
//        if (this->inputController->Keys[GLFW_KEY_W])
    }
}


void Game::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);

    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);

//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//    projection = glm::perspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // Get their uniform location
    // Pass the matrices to the shader
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    shaderProgram->SetMatrix4("model", model);
    shaderProgram->SetMatrix4("view", view);
    shaderProgram->SetMatrix4("projection", projection);

    glBindVertexArray(VAO);

//    log_dbg("OP");

//    GLint vertexColorLocation = glGetUniformLocation(shaderProgram->ID, "color");
//    glUniform4f(vertexColorLocation, glm::sin(2), glm::cos(2), glm::cos(3), 1.0f);

    // Calculate the model matrix for each object and pass it to shader before drawing
//    glm::mat4 model;
//    model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
//    GLfloat angle = 20.0f;
//    model = glm::rotate(model, 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);

//    glBindVertexArray(0);
}


void Game::DoCollisions()
{
}
