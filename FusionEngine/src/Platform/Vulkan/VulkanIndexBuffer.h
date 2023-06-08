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
        void SetData(void* data) override;

        VulkanBuffer* Buffer;
    private:
        Ref<VulkanRenderApi> m_RenderApi;
    };
}
