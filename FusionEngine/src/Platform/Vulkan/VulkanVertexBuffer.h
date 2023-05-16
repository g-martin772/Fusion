#pragma once
#include "Renderer/VertexBuffer.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanBuffer;
    class VulkanRenderApi;
    enum class DrawMode;
}

namespace FusionEngine
{
    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        VulkanVertexBuffer(const std::vector<Attribute>& attributes, uint32_t size);
        ~VulkanVertexBuffer() override;

        void SetData(void* data) override;
        void Bind() override;

        VulkanBuffer* Buffer;
    private:
        Ref<VulkanRenderApi> m_RenderApi;
    };
    
}
