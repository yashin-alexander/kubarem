#ifndef INPUT_H
#define INPUT_H


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

/*
 * All this stuff here is a hook around a glfwSetKeyCallback callback function.
 * We need to store pressed keys somewhere, but callback signature does not
 * provide any `class instance` or `user data` argument. So, _BaseInput class
 * contains glfw callbacks which are redirects all calls to Input class.
*/

class _BaseInput
{
    virtual void _KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mods) = 0;
    virtual void _TextInputCb(GLFWwindow *window, unsigned int codepoint) = 0;

public:
    static _BaseInput *event_handling_instance;

//    virtual ~_BaseInput();

    virtual void setEventHandling();

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


class Input : _BaseInput
{
private:
    GLFWwindow *_window;
    virtual void _KeysSetupCb(GLFWwindow* window, int key, int scancode, int action, int mode);
    virtual void _TextInputCb(GLFWwindow* window, unsigned int codepoint);

public:
    GLboolean Keys[1024] = {GL_FALSE};
    Input(GLFWwindow *window);

//    virtual ~Input();
};



#endif // INPUT_H
