#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <map>
#include <iostream>

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "input.h"


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

        TransformComponent(glm::vec3 position, glm::vec3 size) : position(position), size(size) {
            log_warn("%f", position[0]);
        };

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
        Shader shader;
        ShaderProgramComponent(const char *v_shader_path, const char *f_shader_path) {
            shader = *Shader::LoadFromFile(v_shader_path, f_shader_path);
        };
    };


    struct ModelComponent {
        ModelComponent(string const &path, bool gamma, string const texturePath) :
                model(path, gamma, texturePath) {}
        Model model;
    };

    struct IlluminatedComponent {
        explicit IlluminatedComponent(glm::vec3 light_point) : light_point(light_point) {};
        glm::vec3 light_point;
    };


    struct CameraComponent {
        ThirdPersonCamera camera = ThirdPersonCamera();
        explicit CameraComponent() = default;
    };

    struct InputComponent {
        kubarem::Input input;
        explicit InputComponent(GLFWwindow *window) : input(window) {};
    };

    struct ScreenScaleComponent {
        GLfloat screen_scale;
        explicit ScreenScaleComponent(GLfloat screen_scale) : screen_scale(screen_scale) {};
    };

    struct LoadedModelsComponent {
        std::map<std::string, Model> models_loaded;
        explicit LoadedModelsComponent(std::map<string, Model> models) : models_loaded(std::move(models)) {};
    };
}