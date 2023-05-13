#include "fepch.h"
#include "VulkanBuffer.h"

#include "Renderer/RenderCommand.h"
#include "VulkanRenderApi.h"

namespace FusionEngine
{
    VulkanBuffer::VulkanBuffer(const uint32_t size, vk::MemoryPropertyFlags requestedProperties, vk::BufferUsageFlags usage, vk::SharingMode sharingMode)
        : m_Size(size)
    {
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());

        // Create vk::Buffer
        vk::BufferCreateInfo bufferInfo;
        bufferInfo.flags = vk::BufferCreateFlags();
        bufferInfo.size = m_Size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = sharingMode;

        m_Buffer = m_RenderApi->m_LogicalDevice.createBuffer(bufferInfo);

        // Look for suitable memory on gpu
        const vk::MemoryRequirements memoryRequirements = m_RenderApi->m_LogicalDevice.getBufferMemoryRequirements(m_Buffer);
        // Alignment?

        int32_t memoryTypeIndex = -1;

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

        // Allocate and bind memory
        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = memoryTypeIndex;
        
        m_Memory = m_RenderApi->m_LogicalDevice.allocateMemory(allocateInfo);
        m_RenderApi->m_LogicalDevice.bindBufferMemory(m_Buffer, m_Memory, 0);
    }

    VulkanBuffer::~VulkanBuffer()
    {
        m_RenderApi->m_LogicalDevice.destroyBuffer(m_Buffer);
        m_RenderApi->m_LogicalDevice.freeMemory(m_Memory);
    }
}
