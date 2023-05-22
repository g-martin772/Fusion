#include "SandboxLayer.h"

#include "Renderer/Renderer2D.h"

using namespace FusionEngine;

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnUpdate()
{
    Renderer2D::BeginScene();

    float i = -1.0f, j = -1.0f;
    while (i < 1.0f)
    {
        i += 0.005f;
        j = -1.0f;
        while (j < 1.0f)
        {
            j += 0.005f;
            Renderer2D::DrawQuad({j, i, 0.0f}, {0.002, 0.002}, {1.0f, i, j, 1.0f});
        }
    }

    //Renderer2D::DrawQuad({0.0f, 0.0f, 0.0f}, {0.01, 0.01}, {1.0f, 0.0f, 0.0f, 0.0f});
    //Renderer2D::DrawQuad({0.2f, 0.0f, 0.0f}, {0.01, 0.01}, {1.0f, 0.0f, 0.0f, 0.0f});

    Renderer2D::EndScene();
}

void SandboxLayer::OnDetach()
{
}
