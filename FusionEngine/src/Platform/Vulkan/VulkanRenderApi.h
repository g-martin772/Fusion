#pragma once
#include "Renderer/RenderApi.h"

// https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanResourceManager.h"
#include "VulkanShader.h"
#include "VulkanSwapChain.h"

namespace FusionEngine
{
    class VulkanDevice;

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
        void BeginSwapchainRenderPass() override;
        void EndSwapchainRenderPass() override;
        void Draw(uint32_t vertexCount) override;
        void DrawIndexed(uint32_t indexCount) override;
        void EndFrame() override;

        void WaitDeviceIdle() const { m_Device->Logical().waitIdle(); }
        vk::CommandBuffer GetMainCommandBuffer() const { return m_MainCommandBuffer; }
        vk::CommandBuffer BeginOneTimeCommandBuffer();
        void EndOneTimeCommandBuffer(vk::CommandBuffer commandBuffer);
    private:
        Frame& GetCurrentFrame() { return m_Frames[m_CurrentFrame]; }
    private:
        VulkanInstance* m_Instance;
        VulkanDevice* m_Device;

        // SwapChain
        VulkanSwapChain* m_SwapChain;
        std::vector<Frame> m_Frames;
        uint32_t m_CurrentFrame = 0;
        
        // CommandBuffers
        void CreateCommandPool();
        vk::CommandBuffer CreateCommandBuffer();
        vk::CommandPool m_CommandPool;
        vk::CommandBuffer m_MainCommandBuffer;

        // RenderPass
        void CreateRenderPass();
        vk::RenderPass m_MainRenderPass;

        // Resources
        VulkanResourceManager* m_ResourceManager;
        
        // External
        friend class VulkanPipeline;
        friend class VulkanVertexBuffer;
        friend class VulkanIndexBuffer;
        friend class VulkanShader;
        friend class VulkanSwapChain;
        friend class VulkanBuffer;
        friend class VulkanUniformBuffer;
        friend class VulkanResourceManager;
        friend class VulkanFrameBuffer;
        friend class VulkanImage;
    };

}
