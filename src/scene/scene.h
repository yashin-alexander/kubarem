#pragma once

#include "renderer/renderer.h"

#include <entt/entt.hpp>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace kubarem {
    class Entity;

    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        Entity CreateEntity(const std::string& name = std::string(), const std::string &uuid = "");
        void OnAIUpdateRuntime(float ts);
        void OnUpdateRuntime(float ts);
        void OnRenderRuntime(float ts);
        void InputUpdate();
        entt::registry registry;
    private:
        Renderer renderer;
        friend class Entity;
        py::scoped_interpreter guard{};
    };
}
