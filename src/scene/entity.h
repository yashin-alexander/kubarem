#pragma once

#include <entt/entt.hpp>

#include "log.h"
#include "scene/scene.h"

#include <iostream>

namespace kubarem {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;
        ~Entity() = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args){
            if (hasComponent<T>())
                log_err("%s already exist in entity %d, replacing", typeid(T).name(), entityHandle_);

            T& component = scene_->registry_.emplace_or_replace<T>(entityHandle_, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& getComponent(){
            return scene_->registry_.get<T>(entityHandle_);
        }

        template<typename T>
        bool hasComponent()
        {
            return scene_->registry_.has<T>(entityHandle_);
        }

        template<typename T>
        void removeComponent()
        {
            if (!hasComponent<T>())
                log_err("%s does not exist %s in entity %d", typeid(T).name(), entityHandle_);
            scene_->registry_.remove_if_exists<T>(entityHandle_);
        }
    private:
        entt::entity entityHandle_{0};
        Scene* scene_ = nullptr;
    };
}
