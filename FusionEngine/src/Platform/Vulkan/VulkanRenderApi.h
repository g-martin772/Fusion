#pragma once
#include "Renderer/RenderApi.h"

// https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md
#include <vulkan/vulkan.hpp>

#include "VulkanShader.h"
#include "VulkanSwapChain.h"

namespace FusionEngine
{
    class VulkanRenderApi : public RenderApi
    {
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

        void BeginFrame() override;
        void Draw(uint32_t vertexCount) override;
        void EndFrame() override;
    private:
        // Instance
        void CreateInstance();
        void CheckSupportedExtensions(const std::vector<const char*>& extensions) const;
        void CheckSupportedLayers(const std::vector<const char*>& layers) const;
        void CreateDebugMessenger();

        vk::Instance m_Instance;
        vk::DispatchLoaderDynamic m_DynamicInstanceDispatcher;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;
        std::vector<const char*> m_EnabledLayers {
            "VK_LAYER_KHRONOS_validation"
        };

        // Device
        void CreatePhysicalDevice();
        void CreateLogicalDevice();
        void CreateGraphicsQueue();
        void CreatePresentQueue();
        
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_LogicalDevice;

        vk::Queue m_GraphicsQueue;
        vk::Queue m_PresentQueue;
        std::optional<uint32_t> m_GraphicsFamily;
        std::optional<uint32_t> m_PresentFamily;
        

        // SwapChain
        VulkanSwapChain* m_SwapChain;
        std::vector<VulkanRenderApi::Frame> m_Frames;
        uint32_t m_CurrentFrame = 0;
        

        // RenderPass
        void CreateRenderPass();

        // CommandBuffers
        void CreateCommandPool();
        vk::CommandBuffer CreateCommandBuffer();

        vk::CommandPool m_CommandPool;
        vk::CommandBuffer m_MainCommandBuffer;

        // Pipeline
        vk::PipelineLayout m_PipelineLayout;
        vk::RenderPass m_RenderPass;
        vk::Pipeline m_Pipeline;

        vk::Buffer m_CurrentVertexBuffer;
        vk::DeviceSize m_CurrentVertexBufferOffsets[1] = {0};
        
        // External
        friend class VulkanPipeline;
        friend class VulkanVertexBuffer;
        friend class VulkanShader;
        friend class VulkanSwapChain;
        friend class VulkanBuffer;
    };

}
