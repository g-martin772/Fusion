#include "fepch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer2D.h"

#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "Core/Camera/Camera.h"

namespace FusionEngine
{
    struct CameraData
    {
        glm::mat4 ViewProjection;
    };
    
    struct RenderData2D
    {
        Ref<Shader> QuadShader;
        Ref<Pipeline> QuadPipeline;
        Ref<UniformBuffer> CameraUniformBuffer;
        CameraData* CameraData;
        Ref<Camera> Camera;

        uint32_t QuadVertexSize = 4 * (sizeof(glm::vec3) + sizeof(glm::vec4));
        std::vector<VertexBuffer::Attribute> QuadVertexBufferLayout;
        Ref<VertexBuffer> QuadVertexBuffer;
        char* QuadBufferStart;
        char* QuadBufferCurrent;

        uint32_t QuadIndexSize = 6 * sizeof(uint32_t);
        Ref<IndexBuffer> QuadIndexBuffer;
        uint32_t* QuadIndexBufferStart;
        uint32_t* QuadIndexBufferCurrent;

        uint32_t QuadCount = 0;
        uint32_t MaxQuads = 2000000;
    };

    static RenderData2D* s_Data;
    
    void Renderer2D::Init()
    {
        s_Data = new RenderData2D();
        
        s_Data->QuadShader = Shader::Create("quad");
        
        s_Data->QuadVertexBufferLayout = {
            VertexBuffer::Attribute::Vec3,
            VertexBuffer::Attribute::Vec4
        };

        s_Data->CameraUniformBuffer = UniformBuffer::Create("Camera", sizeof(CameraData), 0, ShaderType::Vertex);
        s_Data->CameraData = new CameraData;
        
        s_Data->CameraUniformBuffer->Bind();
        
        Pipeline::PipelineSpecification quadPipeSpec;
        quadPipeSpec.Shader = s_Data->QuadShader;
        quadPipeSpec.WireFrame = false;
        quadPipeSpec.VertexBufferLayouts = { s_Data->QuadVertexBufferLayout };
        quadPipeSpec.DescriptorSetLayouts = {{0, 1, ShaderType::Vertex, Pipeline::DescriptorType::UniformBuffer}};
        s_Data->QuadPipeline = Pipeline::Create(quadPipeSpec);

        s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->QuadVertexBufferLayout, 4 * s_Data->MaxQuads * s_Data->QuadVertexSize);
        s_Data->QuadBufferStart = new char[4 * s_Data->QuadVertexSize * s_Data->MaxQuads];
        s_Data->QuadBufferCurrent = s_Data->QuadBufferStart;

        s_Data->QuadIndexBuffer = IndexBuffer::Create(s_Data->MaxQuads * 4 * 6);
        s_Data->QuadIndexBufferStart = new uint32_t[6 * s_Data->MaxQuads];
        s_Data->QuadIndexBufferCurrent = s_Data->QuadIndexBufferStart;
    }

    void Renderer2D::ShutDown()
    {
        delete s_Data->CameraData;
        delete[] s_Data->QuadBufferStart;
        delete[] s_Data->QuadIndexBufferStart;
        delete s_Data;
    }

    void Renderer2D::BeginScene(const Ref<Camera>& camera)
    {
        s_Data->Camera = camera;
        s_Data->CameraData->ViewProjection = s_Data->Camera->GetViewProjectionMatrix();
        s_Data->CameraUniformBuffer->SetData(s_Data->CameraData);
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
    {
        const float vertices[] {
            size.x * (- 1.0f) + pos.x , size.y * (+ 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (- 1.0f) + pos.x , size.y * (- 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (+ 1.0f) + pos.x , size.y * (- 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (+ 1.0f) + pos.x , size.y * (+ 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
        };

        uint32_t indexOffset = s_Data->QuadCount * 4;
        
        const uint32_t indices[] {
            0 + indexOffset,
            1 + indexOffset,
            2 + indexOffset,
            0 + indexOffset,
            2 + indexOffset,
            3 + indexOffset
        };

        memcpy(s_Data->QuadBufferCurrent, vertices, s_Data->QuadVertexSize);
        memcpy(s_Data->QuadIndexBufferCurrent, indices, s_Data->QuadIndexSize);
        
        s_Data->QuadBufferCurrent += s_Data->QuadVertexSize;
        s_Data->QuadIndexBufferCurrent += s_Data->QuadIndexSize;
        s_Data->QuadCount++;

        if (s_Data->QuadCount >= s_Data->MaxQuads)
            Flush();
    }

    void Renderer2D::Flush()
    {
        s_Data->QuadPipeline->Bind();
        
        s_Data->QuadVertexBuffer->SetData(s_Data->QuadBufferStart);
        s_Data->QuadVertexBuffer->Bind();
        s_Data->QuadBufferCurrent = s_Data->QuadBufferStart;
        
        s_Data->QuadIndexBuffer->SetData(s_Data->QuadIndexBufferStart);
        s_Data->QuadIndexBuffer->Bind();
        s_Data->QuadIndexBufferCurrent = s_Data->QuadIndexBufferStart;
        
        RenderCommand::DrawIndexed(s_Data->QuadCount * 6);
        s_Data->QuadCount = 0;
    }
}
