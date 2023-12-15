#include "SandboxLayer.h"

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
    m_Camera = FusionEngine::MakeShared<PerspectiveCameraController>(90, 16.0f / 9.0f, 0.001f, 100.0f);
    m_Model = MakeShared<ObjModel>("assets/models/basic/textcube.obj");
    m_Scene = MakeShared<Scene>();

    // Test ECS
    Entity e = m_Scene->CreateEntity();
    e.AddComponent<SpriteRenderComponent>(SpriteRenderComponent{glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}});
}

void EditorLayer::OnUpdate(const Shared<Time> time)
{
    m_Camera->OnUpdate(time->GetDeltaTime());

    m_Scene->RenderScene(m_Camera);

    // 3D
    
    // Renderer::BeginScene(m_Camera);
    //
    // Renderer::DrawObjModel(m_Model);
    //
    // Renderer::EndScene();

    // UI
    
    ImGui::ShowDemoWindow();
    ImGui::ShowMetricsWindow();
}

void EditorLayer::OnDetach()
{
}
