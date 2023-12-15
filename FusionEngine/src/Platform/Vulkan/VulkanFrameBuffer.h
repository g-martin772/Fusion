#pragma once

#include "Renderer/Framebuffer.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi;

    class VulkanFrameBuffer : public Framebuffer
    {
    public:
        VulkanFrameBuffer(const FramebufferSpecification& spec);
        ~VulkanFrameBuffer() override;
        
        void OnResize(uint32_t width, uint32_t height) override;
        void Begin() override;
        void End() override;
        
        [[nodiscard]] Shared<Image> GetCurrentImage() const override;

        [[nodiscard]] vk::RenderPass GetRenderPass() const { return m_RenderPass; }
        [[nodiscard]] vk::Framebuffer GetFramebuffer() const { return m_Framebuffers[m_FrameIndex]; }
    private:
        void CreateFrameBuffers();
    private:
        Shared<VulkanRenderApi> m_RenderApi;
        uint32_t m_Width = 0, m_Height = 0, m_FrameIndex = 0;
        std::vector<vk::Framebuffer> m_Framebuffers;
        vk::RenderPass m_RenderPass;
    };
}
