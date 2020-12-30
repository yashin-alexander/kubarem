#pragma once

#include <entt/entt.hpp>

#include "log.h"
#include "scene/scene.h"


namespace kubarem {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;
        ~Entity() = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            if (HasComponent<T>())
                log_err("Component already exist %s in entity", GetComponent<T>().name.c_str(), entityHandle_);

            T& component = scene_->registry_.emplace<T>(entityHandle_, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& GetComponent(){
            return scene_->registry_.get<T>(entityHandle_);
        }

        template<typename T>
        bool HasComponent()
        {
            return scene_->registry_.has<T>(entityHandle_);
        }

        template<typename T>
        void RemoveComponent()
        {
            if (!HasComponent<T>())
                log_err("Component does not exist %s in entity %d", GetComponent<T>().name.c_str(), entityHandle_);
            scene_->registry_.remove<T>(entityHandle_);
        }
    private:
        entt::entity entityHandle_{0};
        Scene* scene_ = nullptr;
    };
}
