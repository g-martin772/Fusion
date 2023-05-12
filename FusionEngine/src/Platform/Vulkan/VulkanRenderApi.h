#pragma once
#include "Renderer/RenderApi.h"

// https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md
#include <vulkan/vulkan.hpp>

#include "VulkanShader.h"

namespace FusionEngine
{
    class VulkanRenderApi : public RenderApi
    {
        struct SwapChainCapabilities
        {
            vk::SurfaceCapabilitiesKHR Capabilities;
            std::vector<vk::SurfaceFormatKHR> Formats;
            std::vector<vk::PresentModeKHR> PresentModes;
        };

        struct Frame
        {
            vk::Framebuffer FrameBuffer;
            vk::CommandBuffer CommandBuffer;
            vk::Image Image;
            vk::ImageView ImageView;
            vk::Semaphore ImageAvailable, RenderFinished;
            vk::Fence InFlightFence;
        };
        
    public:
        void Init() override;
        void OnWindowResize(uint32_t width, uint32_t height) override;
        void ShutDown() override;

        void Render() override;
    private:
        void CreateInstance();
        void CheckSupportedExtensions(const std::vector<const char*>& extensions) const;
        void CheckSupportedLayers(const std::vector<const char*>& layers) const;
        void CreateDebugMessenger();

        void CreateSurface();

        void CreatePhysicalDevice();
        void CreateLogicalDevice();
        void CreateGraphicsQueue();
        void CreatePresentQueue();

        void QuerySwapchainSupport();
        void CreateSwapChain();
        void RecreateSwapChain();
        void CleanUpSwapChain();

        void CreateRenderPass();

        void CreateFrameBuffers();
        void CreateCommandPool();
        vk::CommandBuffer CreateCommandBuffer();

        void RecordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
    private:
        vk::Instance m_Instance;
        vk::DispatchLoaderDynamic m_DynamicInstanceDispatcher;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;
        std::vector<const char*> m_EnabledLayers {
            "VK_LAYER_KHRONOS_validation"
        };

        vk::SurfaceKHR m_Surface;

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_LogicalDevice;

        vk::Queue m_GraphicsQueue;
        vk::Queue m_PresentQueue;
        std::optional<uint32_t> m_GraphicsFamily;
        std::optional<uint32_t> m_PresentFamily;

        SwapChainCapabilities m_SwapChainCapabilities;
        vk::PresentModeKHR m_PresentMode = vk::PresentModeKHR::eFifo;
        vk::SurfaceFormatKHR m_SurfaceFormat;
        vk::Extent2D m_SwapchainExtent;
        vk::SwapchainKHR m_SwapChain;

        std::vector<Frame> m_Frames;
        uint32_t m_CurrentFrame = 0;

        vk::PipelineLayout m_PipelineLayout;
        vk::RenderPass m_RenderPass;
        vk::Pipeline m_Pipeline;

        vk::CommandPool m_CommandPool;
        vk::CommandBuffer m_MainCommandBuffer;

        
        friend class VulkanPipeline;
    };

}
