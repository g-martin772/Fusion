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


void SandboxLayer::OnAttach()
{
    m_Camera = FusionEngine::MakeRef<PerspectiveCameraController>(90, 16.0f / 9.0f, 0.001f, 100.0f);
    m_Model = MakeRef<ObjModel>("assets/models/basic/textcube.obj");
    m_Scene = MakeRef<Scene>();

    // Test ECS
    Entity e = m_Scene->CreateEntity();
    e.AddComponent<SpriteRenderComponent>(SpriteRenderComponent{glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}});
}

void SandboxLayer::OnUpdate(const Ref<Time> time)
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

void SandboxLayer::OnDetach()
{
}
