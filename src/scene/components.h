#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <soloud.h>

#include <map>
#include <iostream>
#include <random>
#include <sstream>

#include "model.h"
#include "input.h"
#include "shader.h"
#include "camera.h"
#include "uuid.h"
#include "audio/audio.h"
#include "objects/object.h"
#include "particles/particle_controller.h"


namespace kubarem {
    struct UuidComponent {
        std::string uuid;
        explicit UuidComponent(const std::string &uuid) {
            if (uuid.empty())
                this->uuid = UUID::generate_uuid_v4();
            else
                this->uuid = uuid;
        }
    };

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
        explicit ModelComponent(const char *path) :
                model_path(std::string(path)) {}

        std::string model_path;
    };


    struct CameraComponent {
        GLfloat input_speed;
        bool is_tpc;
        Camera camera;
        ThirdPersonCamera tpc_camera;

        explicit CameraComponent(GLfloat input_speed) : camera(), input_speed(input_speed), is_tpc(false) {};
        explicit CameraComponent(GLfloat spring_arm_length, GLfloat input_speed) : tpc_camera(spring_arm_length), input_speed(input_speed), is_tpc(true) {};

        glm::vec3 ProcessKeyboard(CameraMovement direction, float delta_time, GLfloat speed, glm::vec3 position) {
            if (is_tpc)
                return tpc_camera.ProcessKeyboard(direction, delta_time, speed, position);
            else
                return camera.ProcessKeyboard(direction, delta_time, speed, position);
        }

        void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true) {
            if (is_tpc)
                tpc_camera.ProcessMouseMovement(x_offset, y_offset, constrain_pitch);
            else
                camera.ProcessMouseMovement(x_offset, y_offset, constrain_pitch);
        }

        Camera * GetCamera(){
            if (is_tpc)
                return (Camera *)&tpc_camera;
            else
                return &camera;
        }
    };

    struct InputComponent {
        kubarem::Input input;

        explicit InputComponent(GLFWwindow *window) : input(window) {};
    };

    struct AudioCoreComponent {
        SoLoud::Soloud soloud;

        explicit AudioCoreComponent(SoLoud::Soloud soloud) : soloud(soloud) {};
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
        explicit IlluminateCacheComponent(std::vector<glm::vec3> light_sources) : light_sources(
                std::move(light_sources)) {
        };

        std::vector<glm::vec3> light_sources;
    };

    struct ThirdPersonCharacterComponent {
        bool is_third_person_char = true;

        explicit ThirdPersonCharacterComponent(bool is_tpc) : is_third_person_char(is_tpc) {};
    };

    struct CubeObjectComponent {
        GLfloat _vertices[288] = {
                // positions         // normals        // texture coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };
        GLuint VBO_ = 0;
        GLuint VAO_ = 0;
        GLuint texture = 0;
        std::string texture_path;

        explicit CubeObjectComponent(const char *texture_path) : texture_path(texture_path) {
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

        ParticlesComponent(ParticleParameters parameters, uint32_t particles_number) : controller(parameters,
                                                                                                  particles_number) {}
    };

    struct AudioPositionedComponent {
        AudioPositioned audio;
        bool is_playing = false;

        AudioPositionedComponent(SoLoud::Soloud *soloud_core, const char *file_path) : audio(soloud_core, file_path) {};
    };

    struct AudioBackgroundComponent {
        AudioBackground audio;
        bool is_playing = false;

        AudioBackgroundComponent(SoLoud::Soloud *soloud_core, const char *file_path) : audio(soloud_core, file_path) {};
    };

    struct AudioSpeechComponent {
        AudioSpeech audio;
        bool is_playing = false;

        AudioSpeechComponent(SoLoud::Soloud *soloud_core, const char *text_to_speak, unsigned int frequency,
                             float speed, float declination, int wave_form) :
                audio(soloud_core, text_to_speak, frequency, speed, declination, wave_form) {};
    };
}
