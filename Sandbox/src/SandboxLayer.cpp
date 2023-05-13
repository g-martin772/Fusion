#include "fepch.h"
#include "SandboxLayer.h"

#include "Renderer/RenderCommand.h"

using namespace FusionEngine;

void SandboxLayer::OnAttach()
{
    m_VBO = VertexBuffer::Create({ VertexBuffer::Attribute::Vec2, VertexBuffer::Attribute::Vec4 }, DrawMode::Triangles);
    
    Pipeline::PipelineSpecification spec;
    spec.Shader = Shader::Create("test");
    spec.WireFrame = false;
    spec.VertexBuffers = { m_VBO };

    m_PipeLine = Pipeline::Create(spec);
}

void SandboxLayer::OnUpdate()
{
    m_PipeLine->Bind();

    static std::vector<float> vertices = {
         0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.5f, 0.0f, 0.5f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    m_VBO->SetData(vertices.data());
    m_VBO->Bind();
    
    RenderCommand::Render();
}

void SandboxLayer::OnDetach()
{
}
