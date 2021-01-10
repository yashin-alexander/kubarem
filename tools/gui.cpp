#include "gui.h"


void Gui::ImGuiSetup() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    const char *glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Gui::onRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // -------

    static bool dockspace_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
//        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
//        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(window_, GL_TRUE);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Light settings");
    static float col4[4] = {};
    ImGui::ColorEdit4("Color", col4);
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Game Viewport    ");

    viewport_focused = ImGui::IsWindowFocused();
    viewport_hovered = ImGui::IsWindowHovered();

//        io = ImGui::GetIO();
//        e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
//        e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (viewport_size.x != viewportPanelSize.x or viewport_size.y != viewportPanelSize.y) {
        framebuffer_->Resize((uint32_t) viewportPanelSize.x, (uint32_t) viewportPanelSize.y);
        viewport_size = {viewportPanelSize.x, viewportPanelSize.y};
    }

    uint32_t textureID = framebuffer_->GetColorAttachmentRendererID();

    // for now original texture is mirrored
    // updating uv coordinates in here
    // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#about-texture-coordinates
    ImVec2 uv0 = ImVec2(0.0, 1.0); // upper-left portion of the texture
    ImVec2 uv1 = ImVec2(1.0, 0.0); // bottom-right portion of the texture
    ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{viewport_size.x, viewport_size.y}, uv0, uv1);

    ImGui::End();
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::PopStyleVar();
    ImGui::End();

    ImGui::ShowDemoWindow();

    RenderComponentsTree_();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}


void Gui::RenderComponentsTree_() {
    ImGui::Begin("Components tree");

    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    auto components_view = this->scene_->registry.view<kubarem::TagComponent, kubarem::TransformComponent>();

    for (auto entity : components_view) {
        auto [tag, transform] = components_view.get<kubarem::TagComponent, kubarem::TransformComponent>(entity);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.tag.c_str());
        if (open){
            ImGui::InputFloat3("Position", glm::value_ptr(transform.position));
            ImGui::InputFloat3("Size", glm::value_ptr(transform.size));
            ImGui::TreePop();
        }
    }
    ImGui::End();
}


void Gui::Close() {
    log_info("Closing the GUI");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}