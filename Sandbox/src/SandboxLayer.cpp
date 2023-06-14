#include "SandboxLayer.h"

#include "imgui.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/Camera/OrthographicCameraController.h"
#include "Core/Camera/PerspectiveCameraController.h"
#include "IO/FileLoaders/ObjModel.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

using namespace FusionEngine;


void SandboxLayer::OnAttach()
{
    m_Camera = FusionEngine::MakeRef<PerspectiveCameraController>(90, 16.0f / 9.0f, 0.001f, 100.0f);
    m_Model = MakeRef<ObjModel>("assets/models/basic/textcube.obj");
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
