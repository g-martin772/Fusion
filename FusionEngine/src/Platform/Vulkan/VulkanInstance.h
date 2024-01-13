#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    namespace VulkanHelpers
    {
        inline vk::Instance CreateInstance()
        {
            vk::ApplicationInfo applicationInfo{};
            applicationInfo.apiVersion = VK_API_VERSION_1_3;
            applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            applicationInfo.pApplicationName = "Fusion Engine";
            applicationInfo.pEngineName = "Fusion Engine";
            
            vk::InstanceCreateInfo createInfo{};
            createInfo.enabledExtensionCount = 0;
            createInfo.ppEnabledExtensionNames = nullptr;
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
            createInfo.pApplicationInfo = &applicationInfo;

            Log::Info("Creating vulkan instance...");
            const vk::Instance instance = vk::createInstance(createInfo);

            return instance;
        }
    }
}

#endif


// #pragma once
//
// #ifdef FE_RENDER_BACKEND_VULKAN
//
// #include <vulkan/vulkan.hpp>
//
// namespace FusionEngine
// {
//     
// }
//
// #endif
