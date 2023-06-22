#include "EditorLayer.h"

#include "imgui.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/UUID.h"
#include "Core/Camera/OrthographicCameraController.h"
#include "Core/Camera/PerspectiveCameraController.h"
#include "IO/FileLoaders/ObjModel.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

using namespace FusionEngine;

void EditorLayer::OnAttach()
{
    m_Camera = FusionEngine::MakeRef<PerspectiveCameraController>(90, 16.0f / 9.0f, 0.001f, 100.0f);
    m_Scene = MakeRef<Scene>();
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(const Ref<Time> time)
{
    m_Camera->OnUpdate(time->GetDeltaTime());

    m_Scene->RenderScene(m_Camera);

    // Setup Dockspace

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    bool useDockspace = true;
    ImGui::Begin("MyDockSpace", &useDockspace, window_flags);
    ImGui::PopStyleVar(3);
    
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    
    ImGui::Begin("Test Window");
    ImGui::End();
    
    // UI
    ImGui::ShowDemoWindow();
    ImGui::ShowMetricsWindow();

    // End Dockspace
    ImGui::End();
}
