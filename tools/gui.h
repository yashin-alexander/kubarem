#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "scene/scene.h"
#include "scene/entity.h"
#include "scene/components.h"
#include "renderer/framebuffer.h"
#include "scene/scene_serializer.h"


class Gui
{
public:
    Gui(GLFWwindow * window,
        kubarem::Framebuffer * framebuffer,
        kubarem::Scene * scene
    ) : window_(window), framebuffer_(framebuffer), scene_(scene), serializer_(scene_) {};

    ~Gui() = default;
    void ImGuiSetup();
    void onRender();
    void Close();

private:
    void renderComponentsTree_();
    void renderEntityNode(kubarem::Entity entity);

    glm::vec2 viewport_size;
    bool viewport_focused = false;
    bool viewport_hovered = false;

    GLFWwindow * window_;
    kubarem::Framebuffer *framebuffer_;
    kubarem::Scene * scene_;

    kubarem::Entity selection_context_;
    kubarem::SceneSerializer serializer_;
};