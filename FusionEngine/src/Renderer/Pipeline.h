#pragma once
#include "RenderApi.h"
#include "Shader.h"
#include "VertexBuffer.h"

namespace FusionEngine
{
    class Pipeline
    {
    public:
        enum class DescriptorType
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
            DescriptorType Type;
        };
        
        struct PipelineSpecification
        {
            DrawMode DrawMode = DrawMode::Triangles;
            Ref<Shader> Shader;
            bool WireFrame = false;

            std::vector<std::vector<VertexBuffer::Attribute>> VertexBufferLayouts;

            std::vector<DescriptorSetLayout> DescriptorSetLayouts;
        };
    public:
        static Ref<Pipeline> Create(const PipelineSpecification& specification);
        virtual ~Pipeline() = default;

        virtual void Bind() = 0;
    };
}
