#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    struct RendererBackendData
    {
        vk::Instance instance;
        vk::DispatchLoaderDynamic instanceLoader;
        vk::DebugUtilsMessengerEXT debugMessenger;
        vk::PhysicalDevice physicalDevice;
        vk::Device device;
        vk::SurfaceKHR surface;
        uint32_t graphicsQueueIndex, presentQueueIndex, computeQueueIndex, transferQueueIndex;
        vk::Queue graphicsQueue, presentQueue, computeQueue, transferQueue;
    };

    struct RendererBackendWindowData
    {
        
    };
}

#endif 