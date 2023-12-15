#include "fepch.h"
#include "Shader.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanShader.h"

namespace FusionEngine
{
    Shared<Shader> Shader::Create(const std::string& name)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeShared<VulkanShader>(name);
        }

        FE_ASSERT(false, "No Graphics API selected");
        return nullptr;
    }

    Shader::Shader(const std::string& name)
        : m_Name(name)
    {
        
    }

    const std::string& Shader::GetName()
    {
        return m_Name;
    }
}
