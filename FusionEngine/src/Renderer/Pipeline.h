#pragma once
#include "Shader.h"

namespace FusionEngine
{
    class Pipeline
    {
    public:
        static Ref<Pipeline> Create(const Ref<Shader>& shader);

        virtual void Bind() = 0;
    };
}
