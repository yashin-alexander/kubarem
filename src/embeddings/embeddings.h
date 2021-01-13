#pragma once

#include <pybind11/embed.h>
#include <iostream>
#include <string>

#include "scene/components.h"

namespace py = pybind11;


class PyTransformComponent final : public kubarem::TransformComponent {
public:
    using kubarem::TransformComponent::TransformComponent;
    void on_update(float delta_time){ PYBIND11_OVERRIDE_PURE(void, kubarem::TransformComponent, on_update); }
};


PYBIND11_EMBEDDED_MODULE(kubarem, module) {
    py::class_<glm::vec3>(module, "Vec3", py::dynamic_attr())
            .def(py::init<>())
            .def_readwrite("x", &glm::vec3::x)
            .def_readwrite("y", &glm::vec3::y)
            .def_readwrite("z", &glm::vec3::z);

    py::class_<PyTransformComponent>(module, "PyTransformComponent", py::dynamic_attr())
            .def(py::init<glm::vec3, glm::vec3>())
            .def("on_update", &PyTransformComponent::on_update)
            .def_readwrite("position", &PyTransformComponent::position)
            .def_readwrite("size", &PyTransformComponent::size);

}