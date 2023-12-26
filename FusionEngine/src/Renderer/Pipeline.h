#pragma once
#include "Framebuffer.h"
#include "RenderApi.h"
#include "Shader.h"
#include "VertexBuffer.h"

namespace FusionEngine
{
    class Pipeline
    {
    public:
        enum class EDescriptorType
        {
            None = 0,
            UniformBuffer,
            StorageBuffer
        };
        
        struct DescriptorSetLayout
        {
            uint32_t Index;
            uint32_t Count;
            ShaderType Stage;
            EDescriptorType Type;
        };
        
        struct PipelineSpecification
        {
            EDrawMode DrawMode = EDrawMode::Triangles;
            Shared<Shader> MainShader;
            Shared<Framebuffer> FrameBuffer;
            bool WireFrame = false;

            std::vector<std::vector<VertexBuffer::Attribute>> VertexBufferLayouts;

            std::vector<DescriptorSetLayout> DescriptorSetLayouts;
        };
    public:
        static Shared<Pipeline> Create(const PipelineSpecification& specification);
        virtual ~Pipeline() = default;

        virtual void Bind() = 0;
    };
}
