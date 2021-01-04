#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <map>
#include <iostream>

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "objects/object.h"


namespace kubarem {
    struct TagComponent {
        std::string tag;

        TagComponent() = default;

        TagComponent(const TagComponent &) = default;

        explicit TagComponent(const std::string &tag)
                : tag(tag) {}
    };

    struct TransformComponent {
        glm::vec3 position;
        glm::vec3 size;

        TransformComponent(glm::vec3 position, glm::vec3 size) : position(position), size(size) {};

//        TransformComponent(const TransformComponent &) = default;

//        explicit TransformComponent(const glm::vec3 &translation)
//                : Translation(translation) {}

//        glm::mat4 GetTransform() const {
//            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
//
//            return glm::translate(glm::mat4(1.0f), Translation)
//                   * rotation
//                   * glm::scale(glm::mat4(1.0f), Scale);
//        }
    };

    struct ShaderProgramComponent {
        std::string v_shader_path;

        explicit ShaderProgramComponent(const char *v_shader_path) : v_shader_path(v_shader_path) {};
    };


    struct ModelComponent {
        explicit ModelComponent(const char * path) :
                model_path(std::string(path)) {}
        std::string model_path;
    };

    struct IlluminatedComponent {
        explicit IlluminatedComponent(glm::vec3 light_point) : light_point(light_point) {};
        glm::vec3 light_point;
    };


    struct CameraComponent {
        ThirdPersonCamera camera;
        GLfloat input_speed;

        explicit CameraComponent(GLfloat spring_arm_length, GLfloat input_speed) : camera(spring_arm_length), input_speed(input_speed) {}
    };

    struct InputComponent {
        kubarem::Input input;
        explicit InputComponent(GLFWwindow *window) : input(window) {};
    };

    struct ScreenScaleComponent {
        GLfloat screen_scale;

        explicit ScreenScaleComponent(GLfloat screen_scale) : screen_scale(screen_scale) {};
    };

    struct ModelsCacheComponent {
        std::map<std::string, Model> cache;

        explicit ModelsCacheComponent(std::map<string, Model> models) : cache(std::move(models)) {};
    };

    struct ShadersCacheComponent {
        std::map<std::string, Shader> cache;

        explicit ShadersCacheComponent(std::map<string, Shader> shaders) : cache(std::move(shaders)) {};
    };

    struct ThirdPersonCharacterComponent {
        bool is_third_person_char = true;
        explicit ThirdPersonCharacterComponent() = default;
    };
}
