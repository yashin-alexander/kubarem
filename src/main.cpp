#include <iostream>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "game.h"
#include "utils.h"
#include "mesh.h"


const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1000;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "kubarem", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    Input input_controller = Input(window);
    Game kubarem = Game(SCR_WIDTH, SCR_HEIGHT, &input_controller);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log_err("Failed to initialize GLAD");
        return -1;
    }
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//    stbi_set_flip_vertically_on_load(true);

    // Define the viewport dimensions
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);


    kubarem.Init();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    while(!glfwWindowShouldClose(window)){
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        kubarem.ProcessInput(deltaTime);

        kubarem.Render();
//        glfwSetWindowTitle(window, kubarem.gameDescription.c_str());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    log_info("Bye");
}
