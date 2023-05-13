#include "fepch.h"
#include "SandboxLayer.h"

#include "Renderer/RenderCommand.h"

void SandboxLayer::OnAttach()
{
    FusionEngine::Pipeline::PipelineSpecification spec;
    spec.Shader = FusionEngine::Shader::Create("test");
    spec.WireFrame = false;
    m_PipeLine = FusionEngine::Pipeline::Create(spec);
}

void SandboxLayer::OnUpdate()
{
    m_PipeLine->Bind();
    FusionEngine::RenderCommand::Render();
}

void SandboxLayer::OnDetach()
{
    
}
