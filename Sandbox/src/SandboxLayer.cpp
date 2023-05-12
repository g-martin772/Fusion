#include "fepch.h"
#include "SandboxLayer.h"

#include "Renderer/RenderCommand.h"

void SandboxLayer::OnAttach()
{
    m_PipeLine = FusionEngine::Pipeline::Create(FusionEngine::Shader::Create("test"));
}

void SandboxLayer::OnUpdate()
{
    m_PipeLine->Bind();
    FusionEngine::RenderCommand::Render();
}

void SandboxLayer::OnDetach()
{
    
}
