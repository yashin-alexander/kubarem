#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern "C" {
    #include "log.h"
}

/*
 * All this stuff here is a hook around a glfwSetKeyCallback callback function.
 * We need to store ppressed keys somewhere, but callback signature does not
 * provide any `class instance` or `user data` argument. So, _BaseInput class
 * contains glfw callbacks which are redirects all calls to Input class.
*/

#ifndef INPUT_H
#define INPUT_H

class _BaseInput
{
    virtual void _KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mods) = 0;
    virtual void _TextInputCb(GLFWwindow *window, unsigned int codepoint) = 0;

public:
    static _BaseInput *event_handling_instance;

//    virtual ~_BaseInput();

    virtual void setEventHandling()
    {
        event_handling_instance = this;
    }

    static void KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if(event_handling_instance)
            event_handling_instance->_KeysSetupCb(window, key, scancode, action, mods);
    }

    static void TextInputCb(GLFWwindow *window, unsigned int codepoint)
    {
        if(event_handling_instance)
            event_handling_instance->_TextInputCb(window, codepoint);
    }
};


_BaseInput * _BaseInput::event_handling_instance;


class Input : _BaseInput
{
private:
    GLFWwindow *_window;
    virtual void _KeysSetupCb(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (key >= 0 && key < 1024)
        {
            if (action == GLFW_PRESS)
                this->Keys[key] = GL_TRUE;
            else if (action == GLFW_RELEASE)
            {
                this->Keys[key] = GL_FALSE;
            }
        }
    }

    virtual void _TextInputCb(GLFWwindow* window, unsigned int codepoint)
    {
        char key_name;
        key_name = std::cin.narrow(codepoint, '0');
        log_dbg("Key press detected: %c", key_name);
    }

public:
    GLboolean Keys[1024] = {GL_FALSE};

    Input(GLFWwindow *window):
    _window(window)
    {
        this->setEventHandling();
        glfwSetKeyCallback(window, KeysSetupCb);
        glfwSetCharCallback(window, TextInputCb);
    }

//    virtual ~Input();
};

#endif
