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
        auto renderStepView = registry_.view<CameraComponent, InputComponent, ScreenScaleComponent, ModelsCacheComponent, ShadersCacheComponent, IlluminateCacheComponent>();
        auto renderModelsDataView = registry_.view<ShaderProgramComponent, ModelComponent, TransformComponent>(
                entt::exclude<kubarem::ThirdPersonCharacterComponent>);
        auto renderTpcDataView = registry_.view<ShaderProgramComponent, ModelComponent, TransformComponent, ThirdPersonCharacterComponent>();
        auto renderCubeDataView = registry_.view<ShaderProgramComponent, TransformComponent, CubeObjectComponent>();

        for (auto renderStepEntity : renderStepView) {  // single renderStepEntity will be unpacked
            auto[camera, input, screen_scale, models_cache, shaders_cache, lights_cache] = renderStepView.get<CameraComponent, InputComponent, ScreenScaleComponent, ModelsCacheComponent,
            ShadersCacheComponent, IlluminateCacheComponent>(renderStepEntity);

            for (auto renderTpcEntity : renderTpcDataView) {
                auto[shader_path, model_path, transform, tpc] = renderTpcDataView.get<ShaderProgramComponent, ModelComponent, TransformComponent, ThirdPersonCharacterComponent>(
                        renderTpcEntity);

                // process input
                if (input.input.Keys[GLFW_KEY_W]) {
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kForward, ts, camera.input_speed,
                                                                       transform.position);
                }
                if (input.input.Keys[GLFW_KEY_S]) {
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kBackward, ts,
                                                                       camera.input_speed, transform.position);
                }
                if (input.input.Keys[GLFW_KEY_A]) {
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kLeft, ts, camera.input_speed,
                                                                       transform.position);
                }
                if (input.input.Keys[GLFW_KEY_D]) {
                    transform.position = camera.camera.ProcessKeyboard(CameraMovement::kRight, ts, camera.input_speed,
                                                                       transform.position);
                }

                lights_cache.light_sources[0] = transform.position;

                if (input.input.MouseOffsetUpdated) {
                    camera.camera.ProcessMouseMovement(input.input.MouseOffsets[X_OFFSET],
                                                       input.input.MouseOffsets[Y_OFFSET]);
                    input.input.MouseOffsetUpdated = false;
                }

                auto shader_unpack = shaders_cache.cache.find(shader_path.v_shader_path);
                auto shader = shader_unpack->second;

                auto model_unpack = models_cache.cache.find(model_path.model_path);
                auto model = model_unpack->second;

                renderer.RenderThirdPersonCharacter(&camera.camera, screen_scale.screen_scale, &model, &shader,
                                                    lights_cache.light_sources[0], transform.position, transform.size);
            }
            // render models
            for (auto renderDataEntity : renderModelsDataView) {
                auto[shader_path, model_path, transform] = renderModelsDataView.get<ShaderProgramComponent, ModelComponent, TransformComponent>(
                        renderDataEntity);

                auto shader_unpack = shaders_cache.cache.find(shader_path.v_shader_path);
                auto shader = shader_unpack->second;
                auto model_unpack = models_cache.cache.find(model_path.model_path);
                auto model = model_unpack->second;

                renderer.Render(&camera.camera, screen_scale.screen_scale, &model, &shader,
                                lights_cache.light_sources[0],
                                transform.position, transform.size);
            }
            // render cubes
            for (auto renderCubeEntity : renderCubeDataView) {
                auto[shader_path, transform, cube] = renderCubeDataView.get<ShaderProgramComponent, TransformComponent, CubeObjectComponent>(
                        renderCubeEntity);

                auto shader_unpack = shaders_cache.cache.find(shader_path.v_shader_path);
                auto shader = shader_unpack->second;

                renderer.RenderCube(&camera.camera, screen_scale.screen_scale, cube.VAO_, cube.texture, &shader,
                                    lights_cache.light_sources[0], transform.position, transform.size);
            }
        }
    }
}