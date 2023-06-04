﻿#include "fepch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer2D.h"
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

        uint32_t QuadVertexSize = 6 * (3*sizeof(float) + 4*sizeof(float));
        std::vector<VertexBuffer::Attribute> QuadVertexBufferLayout;
        Ref<VertexBuffer> QuadVertexBuffer;
        char* QuadBufferStart;
        char* QuadBufferCurrent;

        uint32_t QuadCount = 0;
        uint32_t MaxQuads = 200000;
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

        s_Data->QuadVertexBuffer = VertexBuffer::Create(s_Data->QuadVertexBufferLayout, 6 * s_Data->MaxQuads * s_Data->QuadVertexSize);
        s_Data->QuadBufferStart = new char[6 * s_Data->QuadVertexSize * s_Data->MaxQuads];
        s_Data->QuadBufferCurrent = s_Data->QuadBufferStart;
    }

    void Renderer2D::ShutDown()
    {
        delete s_Data->CameraData;
        delete[] s_Data->QuadBufferStart;
        delete s_Data;
    }

    void Renderer2D::BeginScene(const Ref<Camera>& camera)
    {
        s_Data->Camera = camera;
        s_Data->CameraData->ViewProjection = s_Data->Camera->GetViewProjectionMatrix();
        s_Data->CameraUniformBuffer->SetData(s_Data->CameraData);
        s_Data->QuadPipeline->Bind();
        RenderCommand::BeginFrame();
    }

    void Renderer2D::EndScene()
    {
        Flush();
        RenderCommand::EndFrame();
    }

    void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
    {
        float vertices[] {
            size.x * (- 1.0f) + pos.x , size.y * (+ 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (- 1.0f) + pos.x , size.y * (- 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (+ 1.0f) + pos.x , size.y * (- 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (- 1.0f) + pos.x , size.y * (+ 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (+ 1.0f) + pos.x , size.y * (- 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
            size.x * (+ 1.0f) + pos.x , size.y * (+ 1.0f) + pos.y , pos.z, color.r, color.g, color.b, color.w,
        };

        memcpy(s_Data->QuadBufferCurrent, vertices, 6 * s_Data->QuadVertexSize);
        
        s_Data->QuadBufferCurrent += s_Data->QuadVertexSize;
        s_Data->QuadCount++;

        if (s_Data->QuadCount >= s_Data->MaxQuads)
            Flush();
    }

    void Renderer2D::Flush()
    {
        s_Data->QuadPipeline->Bind();
        s_Data->QuadVertexBuffer->SetData(s_Data->QuadBufferStart);
        s_Data->QuadBufferCurrent = s_Data->QuadBufferStart;
        s_Data->QuadVertexBuffer->Bind();
        RenderCommand::Draw( s_Data->QuadCount * 6);
        s_Data->QuadCount = 0;
    }
}