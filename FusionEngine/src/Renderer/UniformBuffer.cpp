#include "fepch.h"
#include "UniformBuffer.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanUniformBuffer.h"

namespace FusionEngine
{
    Shared<UniformBuffer> UniformBuffer::Create(const std::string& name, uint32_t size, uint32_t binding, ShaderType shaderType)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeShared<VulkanUniformBuffer>(name, size, binding, shaderType);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
