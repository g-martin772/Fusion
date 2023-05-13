#pragma once
#include "Shader.h"
#include "VertexBuffer.h"

namespace FusionEngine
{
    enum class DrawMode
    {
        None = 0,
        Triangles,
        Lines,
        Points
    };
    
    class Pipeline
    {
    public:
        struct PipelineSpecification
        {
            DrawMode DrawMode = DrawMode::Triangles;
            Ref<Shader> Shader;
            bool WireFrame = false;

            std::vector<Ref<VertexBuffer>> VertexBuffers;
        };
    public:
        static Ref<Pipeline> Create(const PipelineSpecification& specification);

        virtual void Bind() = 0;
    };
}
