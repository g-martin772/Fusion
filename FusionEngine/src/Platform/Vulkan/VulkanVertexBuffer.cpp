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

        uint32_t size = 0;
        for (const Attribute attribute : attributes)
            size += Utils::FusionVertexAttributeToByteSize(attribute);
        size *= Utils::GetDrawModeVertexCount(drawmode);
        
        Buffer = new VulkanBuffer(size, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, vk::BufferUsageFlagBits::eVertexBuffer);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
       delete Buffer;
    }

    void VulkanVertexBuffer::SetData(void* data)
    {
        void* memoryLocation = m_RenderApi->m_LogicalDevice.mapMemory(Buffer->GetMemory(), 0, Buffer->GetSize());
        memcpy(memoryLocation, data, Buffer->GetSize());
        m_RenderApi->m_LogicalDevice.unmapMemory(Buffer->GetMemory());
    }

    void VulkanVertexBuffer::Bind()
    {
        m_RenderApi->m_CurrentVertexBuffer = Buffer->GetBuffer();
    }
}
