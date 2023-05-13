#pragma once
#include "Shader.h"

namespace FusionEngine
{
    class Pipeline
    {
    public:
        enum class DrawMode
        {
            None = 0,
            Triangles,
            Lines,
            Points
        };

        struct PipelineSpecification
        {
            DrawMode DrawMode = DrawMode::Triangles;
            Ref<Shader> Shader;
            bool WireFrame = false;
        };
    public:
        static Ref<Pipeline> Create(const PipelineSpecification& specification);

        virtual void Bind() = 0;
    };
}
