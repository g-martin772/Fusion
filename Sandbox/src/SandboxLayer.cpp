#include "SandboxLayer.h"

#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/Camera/OrthographicCameraController.h"
#include "Renderer/Renderer2D.h"

using namespace FusionEngine;

void SandboxLayer::OnAttach()
{
    m_Camera = FusionEngine::MakeRef<OrthographicCameraController>(-1.0f, 1.0f, -1.0f, 1.0f);
    //m_Camera = MakeRef<Camera>();
}

void SandboxLayer::OnUpdate(const Ref<Time> time)
{
    m_Camera->OnUpdate(time->GetDeltaTime());
    Renderer2D::BeginScene(m_Camera);

    float i = -1.0f, j = -1.0f;
    const float stepSize = 0.01f;

    while (i <= 1.0f)
    {
        j = -1.0f;
        while (j <= 1.0f)
        {
            //Renderer2D::DrawQuad({ j, i, 0.0f }, { stepSize - 0.006, stepSize - 0.006}, { 0.5f, i, j, 1.0f });
            j += stepSize;
        }
        i += stepSize;
    }

    //if(Input::IsKeyPressed(KeyCode::A))
    //    FE_TRACE("A is pressed");

    //if(Input::IsMouseButtonPressed(MouseCode::Left))
    //    FE_TRACE("Left MB is pressed");

    //FE_TRACE("Mouse pos: {0}, {1}", Input::GetMouseX(), Input::GetMouseY());
    
    //Renderer2D::DrawQuad({0.0f, 0.0f, 0.0f}, {0.01, 0.01}, {1.0f, 0.0f, 0.0f, 0.0f});
    Renderer2D::DrawQuad({0.0f, 0.0f, 0.0f}, {0.01, 0.01}, {1.0f, 0.0f, 0.0f, 0.0f});

    Renderer2D::EndScene();
}

void SandboxLayer::OnDetach()
{
}
