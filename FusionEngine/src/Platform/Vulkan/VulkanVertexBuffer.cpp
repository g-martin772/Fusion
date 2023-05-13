#include "fepch.h"
#include "VulkanVertexBuffer.h"
#include "VulkanVertexBuffer.h"

#include "VulkanRenderApi.h"
#include "Renderer/Pipeline.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    namespace Utils
    {
        uint32_t FusionVertexAttributeToByteSize(VertexBuffer::Attribute attribute);

        uint32_t GetDrawModeVertexCount(DrawMode drawmode)
        {
            switch (drawmode) {
                case DrawMode::None: return 1;
                case DrawMode::Triangles: return 3;
                case DrawMode::Lines: return 2;
                case DrawMode::Points: return 1;
            }
        }
        
    }
    
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Attribute>& attributes, const DrawMode drawmode) 
    {
        m_VertexAttributes = attributes;
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());

        vk::BufferCreateInfo bufferInfo;
        bufferInfo.flags = vk::BufferCreateFlags();
        for (const Attribute attribute : attributes)
            m_Size += Utils::FusionVertexAttributeToByteSize(attribute);
        m_Size *= Utils::GetDrawModeVertexCount(drawmode);
        bufferInfo.size = m_Size;
        bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        m_Buffer = m_RenderApi->m_LogicalDevice.createBuffer(bufferInfo);

        const vk::MemoryRequirements memoryRequirements = m_RenderApi->m_LogicalDevice.getBufferMemoryRequirements(m_Buffer);
        // Alignment?

        int32_t memoryTypeIndex = -1;
        const vk::MemoryPropertyFlags requestedProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

        const vk::PhysicalDeviceMemoryProperties memoryProperties = m_RenderApi->m_PhysicalDevice.getMemoryProperties();
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
        {
            const bool supported = memoryRequirements.memoryTypeBits & (1 << i);
            const bool sufficient = (memoryProperties.memoryTypes[i].propertyFlags & requestedProperties) == requestedProperties;
            if(supported && sufficient)
            {
                memoryTypeIndex = i;
                break;
            }
        }
        FE_ASSERT(memoryTypeIndex > -1, "Couldnt find any suatable graphics memory!");
        
        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = memoryTypeIndex;
        
        m_Memory = m_RenderApi->m_LogicalDevice.allocateMemory(allocateInfo);
        m_RenderApi->m_LogicalDevice.bindBufferMemory(m_Buffer, m_Memory, 0);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        m_RenderApi->m_LogicalDevice.destroyBuffer(m_Buffer);
        m_RenderApi->m_LogicalDevice.freeMemory(m_Memory);
    }

    void VulkanVertexBuffer::SetData(void* data)
    {
        void* memoryLocation = m_RenderApi->m_LogicalDevice.mapMemory(m_Memory, 0, m_Size);
        memcpy(memoryLocation, data, m_Size);
        m_RenderApi->m_LogicalDevice.unmapMemory(m_Memory);
    }

    void VulkanVertexBuffer::Bind()
    {
        m_RenderApi->m_CurrentVertexBuffer = m_Buffer;
    }
}
