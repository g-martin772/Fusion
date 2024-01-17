#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#include <vulkan/vulkan.hpp>

#define MAX_FRAMES_IN_FLIGHT 3

namespace FusionEngine
{
    struct RendererBackendData
    {
        vk::Instance instance;
        vk::DispatchLoaderDynamic instanceLoader;
        vk::DebugUtilsMessengerEXT debugMessenger;
        vk::PhysicalDevice physicalDevice;
        vk::Device device;
        uint32_t graphicsQueueIndex, presentQueueIndex, computeQueueIndex, transferQueueIndex;
        vk::Queue graphicsQueue, presentQueue, computeQueue, transferQueue;
    };

    struct FrameData
    {
        vk::Framebuffer framebuffer;
        vk::ImageView imageView;
        vk::CommandBuffer commandBuffer;
        vk::Semaphore imageAvailableSemaphore;
        vk::Semaphore renderFinishedSemaphore;
        vk::Fence inFlightFence;
    };

    struct RendererBackendWindowData
    {
        vk::SurfaceKHR surface;
        vk::SwapchainKHR swapchain;
        uint8_t framesInFlight = 1;
        std::array<FrameData, MAX_FRAMES_IN_FLIGHT> frames;
        vk::CommandPool commandPool;
    };
}

#endif 