#include "fepch.h"
#include "VulkanUniformBuffer.h"

#include "VulkanDevice.h"
#include "VulkanRenderApi.h"
#include "VulkanUtils.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    VulkanUniformBuffer::VulkanUniformBuffer(const std::string& name, const uint32_t size, const uint32_t binding, const ShaderType shaderType)
    {
        m_Name = name;
        m_Binding = binding;
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());
        
        Buffer = new VulkanBuffer(size,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            vk::BufferUsageFlagBits::eUniformBuffer);

        m_LayoutBinding = vk::DescriptorSetLayoutBinding(binding, vk::DescriptorType::eUniformBuffer, 1, Utils::FusionShaderTypeToVkShaderType(shaderType));

        m_RenderApi->m_ResourceManager->AddFrameResource(m_LayoutBinding);
    }

    VulkanUniformBuffer::~VulkanUniformBuffer()
    {
        delete Buffer;
    }

    void VulkanUniformBuffer::Bind()
    {
        m_RenderApi->m_ResourceManager->BindFrameResource(m_Binding, Buffer);
    }

    void VulkanUniformBuffer::SetData(void* data)
    {
        void* memoryLocation = m_RenderApi->m_Device->Logical().mapMemory(Buffer->GetMemory(), 0, Buffer->GetSize());
        memcpy(memoryLocation, data, Buffer->GetSize());
        m_RenderApi->m_Device->Logical().unmapMemory(Buffer->GetMemory());
    }
}
