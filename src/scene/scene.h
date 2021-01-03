#pragma once
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
//        void DestroyEntity(Entity entity);
    private:
        entt::registry registry_;
        friend class Entity;
    };
}
