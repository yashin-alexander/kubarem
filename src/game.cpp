#include "game.h"
#include "log.h"


Game::Game(GLuint width, GLuint height, Input * input ):
State(GAME_MENU), Width(width), Height (height), inputController(input)
{
}

void Game::Init(){
    State = GAME_MENU;
    model = glm::mat4(1.0);
    x_rot = 1.0;

    platform = Kobject(glm::vec2(0, 0), glm::vec2(1, 1));

    shaderProgram = loadShaderFromFile("src/shaders/default_vs.glsl", "src/shaders/default_fs.glsl");

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f,   // top left
        //
    };
    unsigned int EBO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 1);
    glBindVertexArray(0);



//        glm::mat4 model(1.0);
//        model = glm::translate(model, glm::vec3(position, 0.0f));

//        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
//        model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
//        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

//        model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

//        this->shader->SetMatrix4("model", model);
}

void Game::ProcessInput(GLfloat deltaTime){
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
        if (this->inputController->Keys[GLFW_KEY_W])
        {
            log_dbg("W is pressed");
            x_rot -= 0.1;
//            model[0] = glm::vec4(1.0, 0.0, 0.0, x_rot);
//            model[1] = glm::vec4(0.0, 1.0, 0.0, 0.0);
//            model[2] = glm::vec4(0.0, 0.0, 1.0, 0.0);
//            model[3] = glm::vec4(0.0, 0.0, 0.0, 1.0);
        }
        if (this->inputController->Keys[GLFW_KEY_S])
        {
            log_dbg("S is pressed");
            x_rot += 0.1;
//            model[0] = glm::vec4(1.0, 0.0, 0.0, x_rot);
//            model[1] = glm::vec4(0.0, 1.0, 0.0, 0.0);
//            model[2] = glm::vec4(0.0, 0.0, 1.0, 0.0);
//            model[3] = glm::vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
}

void Game::Render()
{
//    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
//    shaderProgram->SetMatrix4("projection", projection);

//    glm::vec2 position(0.5f, 0.5f);
//    glm::mat4 model(1.0);
//    model[0] = glm::vec4(val, 0.0, 0.0, 0.0);
//    model[1] = glm::vec4(0.0, val, 0.0, 0.0);
//    model[2] = glm::vec4(0.0, 0.0, val, 0.0);
//    model[3] = glm::vec4(0.0, 0.0, 0.0, 1.0);
//    log_dbg("MODEL: %f", model[0]);

//    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)

//    model = glm::translate(model, glm::vec3(0.51f, 0.51f, 0.0f)); // Move origin of rotation to center of quad
//    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
//    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f)); // Move origin back
//    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    GLint vertexColorLocation = glGetUniformLocation(shaderProgram->ID, "color");
    glUniform4f(vertexColorLocation, 0.0f, x_rot, x_rot , 1.0f);

//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, cubePositions[i]);



    // Render textured quad
//    this->shader->SetVector3f("spriteColor", color);
//    glBindVertexArray(VAO);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindVertexArray(0);

    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    this->shaderProgram->SetMatrix4("projection", projection);
    this->shaderProgram->SetMatrix4("view", view);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram->ID);
    glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f));
    this->shaderProgram->SetMatrix4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3( 0.5f,  0.5f,  0.0f));
    this->shaderProgram->SetMatrix4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
