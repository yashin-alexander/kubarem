#pragma once

#include <pybind11/embed.h>
#include <iostream>
#include <string>

#include "scene/components.h"

namespace py = pybind11;


class PyTransformComponent final : public kubarem::TransformComponent {
public:
    using kubarem::TransformComponent::TransformComponent;
    void on_create(){ PYBIND11_OVERRIDE_PURE(void, kubarem::TransformComponent, on_create); }
    void on_update(float delta_time){ PYBIND11_OVERRIDE_PURE(void, kubarem::TransformComponent, on_update); }
    void on_destroy(){ PYBIND11_OVERRIDE_PURE(void, kubarem::TransformComponent, on_destroy); }
};


//class PyAudioSpeechComponent final : public kubarem::AudioSpeechComponent {
//public:
//    using kubarem::AudioSpeechComponent::AudioSpeechComponent;
//
//    PyAudioSpeechComponent(std::string text_to_speak) : kubarem::AudioSpeechComponent(text_to_speak) {};
//
//    void on_update(float delta_time){ PYBIND11_OVERRIDE_PURE(void, kubarem::AudioSpeechComponent, on_update); }
//};


PYBIND11_EMBEDDED_MODULE(kubarem, module) {
    py::class_<glm::vec3>(module, "Vec3", py::dynamic_attr())
            .def(py::init<>())
            .def_readwrite("x", &glm::vec3::x)
            .def_readwrite("y", &glm::vec3::y)
            .def_readwrite("z", &glm::vec3::z);

    py::class_<PyTransformComponent>(module, "PyTransformComponent", py::dynamic_attr())
            .def(py::init<glm::vec3, glm::vec3>())
            .def("on_create", &PyTransformComponent::on_create)
            .def("on_update", &PyTransformComponent::on_update)
            .def("on_destroy", &PyTransformComponent::on_destroy)
            .def_readwrite("position", &PyTransformComponent::position)
            .def_readwrite("size", &PyTransformComponent::size);

//    py::class_<PyAudioSpeechComponent>(module, "PyAudioSpeechComponent", py::dynamic_attr())
//            .def(py::init<std::string>())
//            .def("on_update", &PyAudioSpeechComponent::on_update);
}