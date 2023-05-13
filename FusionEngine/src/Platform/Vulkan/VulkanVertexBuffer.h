#pragma once
#include "Renderer/VertexBuffer.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi;
    enum class DrawMode;
}

namespace FusionEngine
{
    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        VulkanVertexBuffer(const std::vector<Attribute>& attributes, DrawMode drawmode);
        ~VulkanVertexBuffer() override;

        vk::Buffer GetBuffer() const { return m_Buffer; }
        void SetData(void* data) override;
        void Bind() override;

    private:
        Ref<VulkanRenderApi> m_RenderApi;
        
        vk::Buffer m_Buffer;
        vk::DeviceMemory m_Memory;
        uint32_t m_Size = 0;
    };
    
}
