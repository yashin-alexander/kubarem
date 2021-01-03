#include "scene/scene.h"
#include "scene/components.h"
#include "scene/entity.h"


namespace kubarem {
    Entity Scene::CreateEntity(const std::string &name) {
        Entity entity = {registry_.create(), this};
//        entity.addComponent<TransformComponent>(glm::vec3());
        auto &tag = entity.addComponent<TagComponent>();
        tag.tag = name.empty() ? "Noname entity" : name;

        return entity;
    }

    void Scene::OnUpdateRuntime(float ts) {
        // update all necessary data
        auto renderStepView = registry_.view<CameraComponent, InputComponent, ScreenScaleComponent>();
        for (auto renderStepEntity : renderStepView) {  // single renderStepEntity will be unpacked

            auto [camera, input, screen_scale] = renderStepView.get<CameraComponent, InputComponent, ScreenScaleComponent>(renderStepEntity);

            /*
            if (input.input.Keys[GLFW_KEY_W]){
                ->ProcessKeyboard(CameraMovement::kForward, deltaTime);
            }
            if (input.input.Keys[GLFW_KEY_S]){
                main_character_->ProcessKeyboard(CameraMovement::kBackward, deltaTime);
            }
            if (input.input.Keys[GLFW_KEY_A]){
                main_character_->ProcessKeyboard(CameraMovement::kLeft, deltaTime);
            }
            if (input.input.Keys[GLFW_KEY_D]){
                main_character_->ProcessKeyboard(CameraMovement::kRight, deltaTime);
            }
             */
        }
    }

    void Scene::OnRenderRuntime(float ts) {
        auto renderStepView = registry_.view<CameraComponent, InputComponent, ScreenScaleComponent>();
        auto renderDataView = registry_.view<ShaderProgramComponent, IlluminatedComponent, ModelComponent, TransformComponent>();

        for (auto renderStepEntity : renderStepView) {  // single renderStepEntity will be unpacked
            auto [camera, input, screen_scale] = renderStepView.get<CameraComponent, InputComponent, ScreenScaleComponent>(renderStepEntity);

//            log_info("==== %d", input.input.MouseOffsetUpdated);
            for (auto renderDataEntity : renderDataView ) {
//                log_info("==== %d", &input.input.MouseOffsetUpdated);
                auto [shader, light, model, transform] = renderDataView.get<ShaderProgramComponent, IlluminatedComponent, ModelComponent, TransformComponent>(renderDataEntity);
                glUseProgram(shader.shader.program_ID_);
//                log_info("Shader program ID = %d", shader.shader.program_ID_);
//                log_info("====posi %f %f %f", transform.position[0], transform.position[1], transform.position[2]);
//                log_info("====size %f %f %f", transform.size[0], transform.size[1], transform.size[2]);
//                log_info("====camera zoom %f", camera.camera.zoom_);
//                log_info("Screen Scale! %f", screen_scale.screen_scale);
//                log_info("Draw indicies meshes size %d", model.model.meshes[0].indices.size());

                shader.shader.SetVector3f("light.position", light.light_point);

                // light properties
                auto time = (GLfloat) glfwGetTime();
                shader.shader.SetVector3f("light.ambient", 1.f, 1.f, 1.f);
                shader.shader.SetVector3f("light.diffuse", 0.1f, cos(2 * time), sin(time));
                shader.shader.SetVector3f("light.specular", 1.0f, .0f, .0f);

                // material properties
                shader.shader.SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
                shader.shader.SetFloat("material.shininess", 256.0f);

                glm::mat4 mod_matrix = glm::mat4(1.0f);

//                mod_matrix = glm::translate(mod_matrix, transform.position);
                mod_matrix = glm::translate(mod_matrix, glm::vec3(transform.position[0], transform.position[1], sin(time) * transform.position[2]));
                mod_matrix = glm::scale(mod_matrix, transform.size);

                shader.shader.SetMatrix4("model", mod_matrix);
                glm::mat4 view = camera.camera.GetViewMatrix();
                shader.shader.SetMatrix4("view", view);
                glm::mat4 projection = glm::perspective(glm::radians(camera.camera.zoom_), screen_scale.screen_scale, 0.1f, 1200.0f);
                shader.shader.SetMatrix4("projection", projection);

                model.model.Draw(shader.shader);
            }
        }
    }
}