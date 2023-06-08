#include "fepch.h"
#include "VulkanIndexBuffer.h"

#include "VulkanBuffer.h"
#include "VulkanRenderApi.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    VulkanIndexBuffer::VulkanIndexBuffer(uint32_t size)
    {
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());
        
        Buffer = new VulkanBuffer(size, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, vk::BufferUsageFlagBits::eIndexBuffer);
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        delete Buffer;
    }

    void VulkanIndexBuffer::Bind()
    {
        m_RenderApi->m_Frames[m_RenderApi->m_CurrentFrame].CommandBuffer.bindIndexBuffer(Buffer->GetBuffer(), 0, vk::IndexType::eUint32);
    }

    void VulkanIndexBuffer::SetData(void* data)
    {
        void* memoryLocation = m_RenderApi->m_LogicalDevice.mapMemory(Buffer->GetMemory(), 0, Buffer->GetSize());
        memcpy(memoryLocation, data, Buffer->GetSize());
        m_RenderApi->m_LogicalDevice.unmapMemory(Buffer->GetMemory());
    }
}
