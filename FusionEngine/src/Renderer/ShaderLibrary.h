#pragma once
#include "Shader.h"

namespace FusionEngine
{
    class ShaderLibrary
    {
    public:
        void AddShader(const std::string& name);
        Ref<Shader> GetShader(const std::string& name);
        bool ContainsShader(const std::string& name) const;
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}

