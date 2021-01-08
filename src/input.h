#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

namespace kubarem {

    enum MousePositionOffsets {
        X_OFFSET,
        Y_OFFSET
    };

/*
 * All this stuff here is a hook around a glfwSetKeyCallback callback function.
 * We need to store pressed keys somewhere, but callback signature does not
 * provide any `class instance` or `user data` argument. So, _BaseInput class
 * contains glfw callbacks which are redirects all calls to Input class.
*/

    class _BaseInput {
    protected:
        virtual void _KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mods) = 0;

        virtual void _TextInputCb(GLFWwindow *window, unsigned int codepoint) = 0;

        virtual void _MouseMoveCb(GLFWwindow *window, float xpos, float ypos) = 0;

    public:
        static _BaseInput *event_handling_instance;

//    virtual ~_BaseInput();

        virtual void setEventHandling();

        static void KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (event_handling_instance)
                event_handling_instance->_KeysSetupCb(window, key, scancode, action, mods);
        }

        static void TextInputCb(GLFWwindow *window, unsigned int codepoint) {
            if (event_handling_instance)
                event_handling_instance->_TextInputCb(window, codepoint);
        }

        static void MouseMoveCb(GLFWwindow *window, double xpos, double ypos) {
            if (event_handling_instance)
                event_handling_instance->_MouseMoveCb(window, (float) xpos, (float) ypos);
        }
    };


    class Input : public _BaseInput {
    private:
        bool _cursorVisible = false;
        GLFWwindow *_window;

        virtual void _KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mode) override;

        virtual void _TextInputCb(GLFWwindow *window, unsigned int codepoint) override;

        virtual void _MouseMoveCb(GLFWwindow *window, float xpos, float ypos) override;

        GLfloat _mouseCurrentPositionX = 0.0f;
        GLfloat _mouseCurrentPositionY = 0.0f;

    public:
        GLboolean Keys[1024] = {GL_FALSE};
        GLfloat MouseOffsets[2] = {0.0f, 0.0f};
        GLboolean MouseOffsetUpdated = false;

        Input(GLFWwindow *window);
        void SetCursorVisible();
        void SetCursorInvisible();
        bool IsCursorVisible() const;

//    virtual ~Input();
    };
}