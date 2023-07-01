#pragma once

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi;

    class VulkanBuffer
    {
    public:
        VulkanBuffer(uint32_t size, vk::MemoryPropertyFlags requestedProperties, vk::BufferUsageFlags usage, vk::SharingMode sharingMode = vk::SharingMode::eExclusive);
        ~VulkanBuffer();

        [[nodiscard]] vk::Buffer GetBuffer() const { return m_Buffer; }
        [[nodiscard]] vk::DeviceMemory GetMemory() const { return m_Memory; }
        [[nodiscard]] uint32_t GetSize() const { return m_Size; }
    private:
        Ref<VulkanRenderApi> m_RenderApi;
        
        vk::Buffer m_Buffer;
        vk::DeviceMemory m_Memory;
        uint32_t m_Size = 0;
    };
}
