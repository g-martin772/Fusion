#include "EditorLayer.h"

#include "imgui.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/UUID.h"
#include "Core/Camera/OrthographicCameraController.h"
#include "Core/Camera/PerspectiveCameraController.h"
#include "IO/FileLoaders/ObjModel.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "UI/ImGui.h"

using namespace FusionEngine;

void EditorLayer::OnAttach()
{
    ImageSpecification imageSpecification;
    imageSpecification.Width = 100;
    imageSpecification.Height = 100;
    m_ViewportImage = Image::Create(imageSpecification);

    FramebufferSpecification framebufferSpecification;
    FrameBufferAttachment colorAttachment;
    colorAttachment.Image = m_ViewportImage;
    framebufferSpecification.Attachments = { {colorAttachment} };
    framebufferSpecification.Width = 100;
    framebufferSpecification.Height = 100;
    framebufferSpecification.ClearColor = {0.5f, 0.25f, 0.1f, 1.0f};
    m_ViewportFramebuffer = Framebuffer::Create(framebufferSpecification);

    m_ViewportTextureID = UI::ImGuiGetImageHandle(m_ViewportImage);

    // Setup scene
    m_Scene = MakeRef<Scene>();
    m_Camera = FusionEngine::MakeRef<PerspectiveCameraController>(90, 16.0f / 9.0f, 0.001f, 100.0f);
    Entity e = m_Scene->CreateEntity("Box");
    e.AddComponent<SpriteRenderComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
}

void EditorLayer::OnDetach()
{
    UI::ImGuiFreeImageHandle(m_ViewportTextureID);
}

void EditorLayer::OnUpdate(const Ref<Time> time)
{
    RenderCommand::BeginSwapchainRenderPass();
    
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

    // ImGui Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoTitleBar;
    ImGui::Begin("Viewport", nullptr, viewportFlags);
    ImGui::PopStyleVar();
    const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    const glm::uvec2 newViewportSize = { static_cast<uint32_t>(viewportPanelSize.x), static_cast<uint32_t>(viewportPanelSize.y) };
    if(newViewportSize != m_ViewportSize)
    {
        m_Camera->OnResize(newViewportSize.x, newViewportSize.y);
        m_ViewportFramebuffer->OnResize(newViewportSize.x, newViewportSize.y);
        UI::ImGuiFreeImageHandle(m_ViewportTextureID);
        m_ViewportTextureID = UI::ImGuiGetImageHandle(m_ViewportImage);
    }
    ImGui::Image(m_ViewportTextureID, viewportPanelSize);
    bool viewportFocus = ImGui::IsWindowFocused();
    ImGui::End();
    
    // UI
    ImGui::ShowDemoWindow();
    ImGui::ShowMetricsWindow();

    // End Dockspace
    ImGui::End();

    RenderCommand::EndSwapchainRenderPass();


    
    // Render viewport
    
    m_ViewportFramebuffer->Begin();
    
    if(viewportFocus)
        m_Camera->OnUpdate(time->GetDeltaTime());

    m_Scene->RenderScene(m_Camera);

    m_ViewportFramebuffer->End();
}
