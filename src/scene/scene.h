#pragma once

#include "renderer/renderer.h"

#include <entt/entt.hpp>

namespace kubarem {
    class Entity;

    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        Entity CreateEntity(const std::string& name = std::string());
        void OnUpdateRuntime(float ts);
        void OnRenderRuntime(float ts);
        void InputUpdate();
        entt::registry registry;
    private:
        Renderer renderer;
        friend class Entity;
    };
}
