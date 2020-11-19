#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"
#include "game.h"
#include "utils.h"


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 700;

void glfwErrorCallback(int error, const char* description)
{
    log_err("GLFW Error %d: %s", error, description);
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);

    assert(glfwInit() == GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "kubarem", nullptr, nullptr);
    assert(window != nullptr);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    kubarem.Init();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    while(!glfwWindowShouldClose(window)){
        GLfloat currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        kubarem.ProcessInput(deltaTime);
        kubarem.DoCollisions();
        kubarem.Render(deltaTime);
//        glfwSetWindowTitle(window, kubarem.gameDescription.c_str());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    log_info("Bye");
}
