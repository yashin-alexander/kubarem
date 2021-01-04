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
    private:
        entt::registry registry_;
        Renderer renderer;
        friend class Entity;
    };
}
