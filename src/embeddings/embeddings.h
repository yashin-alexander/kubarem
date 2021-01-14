#pragma once

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>

#include "log.h"
#include "scene/components.h"

namespace py = pybind11;


class PyEntity {
public:
    kubarem::TagComponent tag;
    kubarem::UuidComponent uuid;
    kubarem::TransformComponent transform;
    PyEntity(kubarem::UuidComponent uuid, kubarem::TagComponent tag, kubarem::TransformComponent transform) : uuid(uuid), tag(tag), transform(transform) {};

    void on_destroy(){
        log_dbg("On destroy event called on entity '%s'", this->tag.tag.c_str());
    }

    void on_update(float delta_time){}

    void on_create(){
        log_dbg("On create event called on entity '%s'", this->tag.tag.c_str());
    };
};


class PyScene {
public:
    std::vector<PyEntity> entities;
    explicit PyScene(std::vector<PyEntity> entities) : entities(entities){};
};


PYBIND11_EMBEDDED_MODULE(kubarem, module) {
    py::class_<glm::vec3>(module, "Vec3", py::dynamic_attr())
            .def(py::init<float, float, float>())
            .def_readwrite("x", &glm::vec3::x)
            .def_readwrite("y", &glm::vec3::y)
            .def_readwrite("z", &glm::vec3::z);

    py::class_<kubarem::TransformComponent>(module, "TransformComponent", py::dynamic_attr())
            .def(py::init<glm::vec3, glm::vec3>())
            .def_readwrite("position", &kubarem::TransformComponent::position)
            .def_readwrite("size", &kubarem::TransformComponent::size);

    py::class_<kubarem::TagComponent>(module, "TagComponent", py::dynamic_attr())
            .def(py::init<std::string>())
            .def_readwrite("tag", &kubarem::TagComponent::tag);

    py::class_<kubarem::UuidComponent>(module, "UuidComponent", py::dynamic_attr())
            .def(py::init<std::string>())
            .def_readonly("uuid", &kubarem::UuidComponent::uuid);

    py::class_<PyEntity>(module, "PyEntity", py::dynamic_attr())
            .def(py::init<kubarem::UuidComponent, kubarem::TagComponent, kubarem::TransformComponent>())
            .def("on_create", &PyEntity::on_create)
            .def("on_update", &PyEntity::on_update)
            .def("on_destroy", &PyEntity::on_destroy)
            .def_readonly("uuid", &PyEntity::uuid)
            .def_readonly("tag", &PyEntity::tag)
            .def_readwrite("transform", &PyEntity::transform);

    py::class_<PyScene>(module, "PyScene", py::dynamic_attr())
            .def(py::init<std::vector<PyEntity>>())
            .def_readwrite("entities", &PyScene::entities);
}