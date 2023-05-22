#pragma once
#include "VulkanBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"

namespace FusionEngine
{
    class VulkanUniformBuffer : public UniformBuffer
    {
    public:
        VulkanUniformBuffer(const std::string& name, uint32_t size, uint32_t binding, ShaderType shaderType);
        ~VulkanUniformBuffer() override;
        
        void Bind() override;
        void SetData(void* data) override;

        [[nodiscard]] vk::DescriptorSetLayoutBinding GetLayoutBinding() const { return m_LayoutBinding; }

        VulkanBuffer* Buffer;
    private:
        Ref<VulkanRenderApi> m_RenderApi;
        uint32_t m_Binding;

        vk::DescriptorSetLayoutBinding m_LayoutBinding;
    };
}
