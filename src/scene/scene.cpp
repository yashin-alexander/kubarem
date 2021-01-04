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
    }

    void Scene::OnRenderRuntime(float ts) {
        auto renderStepView = registry_.view<CameraComponent, InputComponent, ScreenScaleComponent, ModelsCacheComponent, ShadersCacheComponent>();
        auto renderDataView = registry_.view<ShaderProgramComponent, IlluminatedComponent, ModelComponent, TransformComponent>(
                entt::exclude<kubarem::ThirdPersonCharacterComponent>);
        auto renderTpcDataView = registry_.view<ShaderProgramComponent, IlluminatedComponent, ModelComponent, TransformComponent, ThirdPersonCharacterComponent>();

        for (auto renderStepEntity : renderStepView) {  // single renderStepEntity will be unpacked
            auto[camera, input, screen_scale, models_cache, shaders_cache] = renderStepView.get<CameraComponent, InputComponent, ScreenScaleComponent, ModelsCacheComponent, ShadersCacheComponent>(
                    renderStepEntity);

            for (auto renderDataEntity : renderDataView) {
                auto[shader_path, model_path, light, transform] = renderDataView.get<ShaderProgramComponent, ModelComponent, IlluminatedComponent, TransformComponent>(
                        renderDataEntity);

                // pull shader and model from entt cache
                auto shader_unpack = shaders_cache.cache.find(shader_path.v_shader_path);
                auto shader = shader_unpack->second;

                auto model_unpack = models_cache.cache.find(model_path.model_path);
                auto model = model_unpack->second;

                renderer.Render(&camera.camera, screen_scale.screen_scale, &model, &shader, light.light_point,
                                transform.position, transform.size);
            }

            for (auto renderTpcEntity : renderTpcDataView) {
                auto[shader_path, model_path, light, transform, tpc] = renderTpcDataView.get<ShaderProgramComponent, ModelComponent, IlluminatedComponent, TransformComponent, ThirdPersonCharacterComponent>(
                        renderTpcEntity);

                if (input.input.Keys[GLFW_KEY_W]){
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kForward, ts, camera.input_speed, transform.position);
                }
                if (input.input.Keys[GLFW_KEY_S]){
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kBackward, ts, camera.input_speed, transform.position);
                }
                if (input.input.Keys[GLFW_KEY_A]){
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kLeft, ts, camera.input_speed, transform.position);
                }
                if (input.input.Keys[GLFW_KEY_D]){
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kRight, ts, camera.input_speed, transform.position);
                }

                if (input.input.MouseOffsetUpdated){
                    camera.camera.ProcessMouseMovement(input.input.MouseOffsets[X_OFFSET], input.input.MouseOffsets[Y_OFFSET]);
                    input.input.MouseOffsetUpdated = false;
                }

                auto shader_unpack = shaders_cache.cache.find(shader_path.v_shader_path);
                auto shader = shader_unpack->second;

                auto model_unpack = models_cache.cache.find(model_path.model_path);
                auto model = model_unpack->second;

                renderer.RenderThirdPersonCharacter(&camera.camera, screen_scale.screen_scale, &model, &shader, light.light_point, transform.position, transform.size);
            }
        }
    }
}