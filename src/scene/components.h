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
#include "particles/particle_controller.h"


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

    struct IlluminateCacheComponent {
        explicit IlluminateCacheComponent(std::vector<glm::vec3> light_sources) : light_sources(std::move(light_sources)) {};

        std::vector<glm::vec3> light_sources;
    };

    struct ThirdPersonCharacterComponent {
        bool is_third_person_char = true;
        explicit ThirdPersonCharacterComponent() = default;
    };

    struct CubeObjectComponent {
        GLfloat _vertices[288] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };
        GLuint VBO_ = 0;
        GLuint VAO_ = 0;
        GLuint texture = 0;

        explicit CubeObjectComponent(const char * texture_path) {
            glGenVertexArrays(1, &VAO_);
            glGenBuffers(1, &VBO_);

            glBindVertexArray(VAO_);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
            // normales coord attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // texture
            loadTexture_(texture_path);
        };

        void loadTexture_(char const *path) {
            glGenTextures(1, &texture);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
            if (data) {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;
                else {
                    log_err("CustomGeometryObject: unable to determine texture format");
                    format = GL_RED;
                }

                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            } else {
                log_err("Failed to load texture %s", path);
                stbi_image_free(data);
            }
        }
    };

    struct ParticlesComponent {
        ParticleController controller;

        ParticlesComponent(ParticleParameters parameters, uint32_t particles_number) : controller(parameters, particles_number) {}
    };
}
