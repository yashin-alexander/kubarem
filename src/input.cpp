#include "input.h"

namespace kubarem {

    _BaseInput *_BaseInput::event_handling_instance;

    void _BaseInput::setEventHandling() {
        event_handling_instance = this;
    }


    void Input::_KeysSetupCb(GLFWwindow *window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (key >= 0 && key < 1024) {
            if (action == GLFW_PRESS)
                this->Keys[key] = GL_TRUE;
            else if (action == GLFW_RELEASE) {
                this->Keys[key] = GL_FALSE;
            }
        }
    }

    void Input::_TextInputCb(GLFWwindow *window, unsigned int codepoint) {
        char key_name;
        key_name = std::cin.narrow(codepoint, '0');
//    log_dbg("Key press detected: %c", key_name);
    }


    void Input::_MouseMoveCb(GLFWwindow *window, float xpos, float ypos) {
        MouseOffsets[X_OFFSET] = xpos - _mouseCurrentPositionX;
        MouseOffsets[Y_OFFSET] = _mouseCurrentPositionY - ypos;

        _mouseCurrentPositionX = xpos;
        _mouseCurrentPositionY = ypos;

        MouseOffsetUpdated = true;
    }

    Input::Input(GLFWwindow *window) :
            _window(window) {
        this->setEventHandling();
        glfwSetKeyCallback(window, KeysSetupCb);
        glfwSetCharCallback(window, TextInputCb);
        glfwSetCursorPosCallback(window, MouseMoveCb);
    }

    void Input::SetCursorVisible() {
        if (!this->_cursorVisible){
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            this->_cursorVisible = true;
            log_info("Cursor is enabled");
        }
    }

    void Input::SetCursorInvisible() {
        if (this->_cursorVisible){
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            this->_cursorVisible = false;
            log_info("Cursor is disabled");
        }
    }

    bool Input::IsCursorVisible() const {
        return this->_cursorVisible;
    }
}