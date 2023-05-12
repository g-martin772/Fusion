#include "fepch.h"
#include "ShaderLibrary.h"

namespace FusionEngine
{
    void ShaderLibrary::AddShader(const std::string& name)
    {
        m_Shaders[name] = Shader::Create(name);
    }

    Ref<Shader> ShaderLibrary::GetShader(const std::string& name)
    {
        return m_Shaders.at(name);
    }

    bool ShaderLibrary::ContainsShader(const std::string& name) const
    {
        return m_Shaders.count(name);
    }
}

