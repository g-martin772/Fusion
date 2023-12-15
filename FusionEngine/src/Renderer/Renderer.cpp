#include "fepch.h"
#include "Renderer.h"

#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "Core/Camera/Camera.h"
#include "IO/FileLoaders/ObjModel.h"

namespace FusionEngine
{
    struct CameraData
    {
        glm::mat4 ViewProjection;
    };

    struct RenderData
    {
       
    };

    static RenderData* s_Data;

    void Renderer::Init()
    {
        s_Data = new RenderData();
    }

    void Renderer::Shutdown()
    {
        delete s_Data;
    }

    void Renderer::BeginScene(const Shared<Camera>& camera)
    {
        
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::DrawObjModel(Shared<ObjModel> model)
    {
        model->GetVertexBuffer()->Bind();
        model->GetIndexBuffer()->Bind();
        RenderCommand::DrawIndexed(model->GetIndexCount());
    }
}
