#pragma once
#include "Renderer/IndexBuffer.h"

namespace FusionEngine
{
    class VulkanBuffer;
    class VulkanRenderApi;
    
    
    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        VulkanIndexBuffer(uint32_t size);
        ~VulkanIndexBuffer() override;
        
        void Bind() override;
        void SetData(void* data, uint32_t size) override;

        VulkanBuffer* Buffer;
    private:
        Shared<VulkanRenderApi> m_RenderApi;
    };
}
