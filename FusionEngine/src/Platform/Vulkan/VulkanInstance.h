#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#include "VulkanRendererBackendData.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine { namespace VulkanHelpers {
    inline VKAPI_ATTR VkBool32 VKAPI_CALL VulkanMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        switch(messageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                Log::Trace("Vulkan validation layer: {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                Log::Info("Vulkan validation layer: {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                Log::Warn("Vulkan validation layer: {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                Log::Error("Vulkan validation layer: {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                Log::Error("Vulkan validation layer: {0}", pCallbackData->pMessage);
                break;
        }
        
        return VK_FALSE;
    }

    inline vk::Instance CreateInstance()
    {
        vk::ApplicationInfo applicationInfo{};
        applicationInfo.apiVersion = VK_API_VERSION_1_3;
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pApplicationName = "Fusion Engine";
        applicationInfo.pEngineName = "Fusion Engine";

        const List<const char*> extensions = {
            "VK_EXT_debug_utils",
            "VK_KHR_surface",
#ifdef FE_WINDOWS
            "VK_KHR_win32_surface",
#endif
        };
        
        const List<const char*> layers = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        vk::InstanceCreateInfo createInfo{};
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.pApplicationInfo = &applicationInfo;

        Log::Info("Creating vulkan instance...");
        return vk::createInstance(createInfo);
    }

    inline vk::DebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT(const RendererBackendData* data)
    {
        vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
        createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                 vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding;
        createInfo.pfnUserCallback = VulkanMessageCallback;
        return data->instance.createDebugUtilsMessengerEXT(createInfo, nullptr, data->instanceLoader);
    }
} }

#endif


// #pragma once
//
// #ifdef FE_RENDER_BACKEND_VULKAN
//
// #include "VulkanRendererBackendData.h"
//
// #include <vulkan/vulkan.hpp>
//
// namespace FusionEngine { namespace VulkanHelpers {
// 
//     
// } }
//
// #endif
