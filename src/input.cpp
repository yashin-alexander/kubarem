#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Input{
private:
    GLFWwindow *_window;

    static void _TextInputCb(GLFWwindow* window, unsigned int codepoint)
    {
        char key_name;
        key_name = std::cin.narrow(codepoint, '0');
        std::cout << "Key press detected: " << key_name << std::endl;
    }

    static void _KeysSetupCb(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

public:
    Input(GLFWwindow *window):
    _window(window)
    {
        glfwSetKeyCallback(this->_window, this->_KeysSetupCb);
        glfwSetCharCallback(this->_window, this->_TextInputCb);
    }
};
