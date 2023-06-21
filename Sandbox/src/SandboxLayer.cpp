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
    Entity e = m_Scene->CreateEntity("MyTestEntity");
    UUID uuid = e.GetUUID();
    FE_INFO("UUID: {0}", uuid.ToString());

    TransformComponent& tc = e.GetComponent<TransformComponent>();
    tc.Position = {1.0f, 2.0f, 3.0f};
    tc = e.GetComponent<TransformComponent>();
    FE_INFO("Position: {0}, {1}, {2}", tc.Position.x, tc.Position.y, tc.Position.z);

    Entity foundEntity = m_Scene->GetEntityByUUID(uuid);
    FE_INFO("Found entity with UUID: {0}", foundEntity.GetUUID().ToString());
    
}

void SandboxLayer::OnUpdate(const Ref<Time> time)
{
    m_Camera->OnUpdate(time->GetDeltaTime());

    // 2D
    
    Renderer2D::BeginScene(m_Camera);
    
  
    Renderer2D::DrawQuad({0.0f, 0.0f, 0.0f}, {0.01, 0.01}, {1.0f, 0.0f, 0.0f, 0.0f});

    Renderer2D::EndScene();

    // 3D
    
    Renderer::BeginScene(m_Camera);

    Renderer::DrawObjModel(m_Model);
    
    Renderer::EndScene();

    // UI
    
    ImGui::ShowDemoWindow();
    ImGui::ShowMetricsWindow();
}

void SandboxLayer::OnDetach()
{
}
