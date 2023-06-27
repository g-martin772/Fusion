#pragma once

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi;

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain();
        ~VulkanSwapChain();

        void AcquireSurfaceCapabilities();
        void InitializeSwapchain();
        void RecreateSwapChain();

        [[nodiscard]] uint32_t GetImageCount() const { return m_Capabilities.minImageCount + 1; }
        
        [[nodiscard]] vk::SurfaceKHR GetSurface() const { return m_Surface; }
        [[nodiscard]] vk::PresentModeKHR GetPresentMode() const { return m_PresentMode; }
        [[nodiscard]] vk::SurfaceFormatKHR GetSurfaceFormat() const { return m_SurfaceFormat; }
        [[nodiscard]] vk::Extent2D GetSwapChainExtent() const { return m_SwapchainExtent; }
        [[nodiscard]] vk::SwapchainKHR GetSwapChain() const { return m_SwapChain; }
    private:
        void CreateSwapChain();
        void CleanUpSwapChain();
        void CreateFrameBuffers();
    private:
        Ref<VulkanRenderApi> m_RenderApi;

        vk::SurfaceKHR m_Surface;

        vk::PresentModeKHR m_PresentMode = vk::PresentModeKHR::eFifo;
        vk::SurfaceFormatKHR m_SurfaceFormat;
        vk::Extent2D m_SwapchainExtent;
        vk::SwapchainKHR m_SwapChain;
        std::vector<vk::SurfaceFormatKHR> m_Formats;
        vk::SurfaceCapabilitiesKHR m_Capabilities;
    };
    
}
