#include "fepch.h"
#include "VulkanVertexBuffer.h"
#include "VulkanVertexBuffer.h"

#include "VulkanBuffer.h"
#include "VulkanRenderApi.h"
#include "Renderer/Pipeline.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    namespace Utils
    {
        uint32_t FusionVertexAttributeToByteSize(VertexBuffer::Attribute attribute);
    }
    
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Attribute>& attributes, uint32_t size) 
    {
        m_VertexAttributes = attributes;
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());
        
        Buffer = new VulkanBuffer(size, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, vk::BufferUsageFlagBits::eVertexBuffer);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        m_RenderApi->m_LogicalDevice.unmapMemory(Buffer->GetMemory());
       delete Buffer;
    }

    void VulkanVertexBuffer::SetData(void* data, uint32_t size)
    {
        FE_ASSERT(size <= Buffer->GetSize(), "Data is larger than buffer size");
        if(!m_MappedMemory)
            m_MappedMemory = m_RenderApi->m_LogicalDevice.mapMemory(Buffer->GetMemory(), 0, Buffer->GetSize());
        memcpy(m_MappedMemory, data, size);
    }

    void VulkanVertexBuffer::Bind()
    {
        static vk::DeviceSize currentVertexBufferOffsets[1] = {0};
        m_RenderApi->m_Frames[m_RenderApi->m_CurrentFrame].CommandBuffer.bindVertexBuffers(0, 1, &Buffer->GetBuffer(), currentVertexBufferOffsets);
    }
}
