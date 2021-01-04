#pragma once

#include "model.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "shader.h"
#include "camera.h"


class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;
    void Render(ThirdPersonCamera *camera, GLfloat screen_scale,  Model *model, Shader *shader, glm::vec3 light_point, glm::vec3 position, glm::vec3 size);
    void Render(ThirdPersonCamera *camera, GLfloat screen_scale,  Model *model, Shader *shader, glm::vec3 light_point, glm::vec3 position, glm::vec3 size, GLfloat delta_time);
    void RenderThirdPersonCharacter(ThirdPersonCamera *camera, GLfloat screen_scale,  Model *model, Shader *shader, glm::vec3 light_point, glm::vec3 position, glm::vec3 size);

private:
};