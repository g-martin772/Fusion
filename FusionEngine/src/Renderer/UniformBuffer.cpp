#include "fepch.h"
#include "UniformBuffer.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanUniformBuffer.h"

namespace FusionEngine
{
    Ref<UniformBuffer> UniformBuffer::Create(const std::string& name, uint32_t size, uint32_t binding, ShaderType shaderType)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeRef<VulkanUniformBuffer>(name, size, binding, shaderType);
        }
    }
}
