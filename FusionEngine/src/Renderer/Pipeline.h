#pragma once
#include "RenderApi.h"
#include "Shader.h"
#include "VertexBuffer.h"

namespace FusionEngine
{
    class Pipeline
    {
    public:
        struct PipelineSpecification
        {
            DrawMode DrawMode = DrawMode::Triangles;
            Ref<Shader> Shader;
            bool WireFrame = false;

            std::vector<std::vector<VertexBuffer::Attribute>> VertexBufferLayouts;
        };
    public:
        static Ref<Pipeline> Create(const PipelineSpecification& specification);

        virtual void Bind() = 0;
    };
}
