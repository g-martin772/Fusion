#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#if defined(FE_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "VulkanRendererBackendData.h"
#include "Core/Application.h"

namespace FusionEngine::VulkanHelpers
{
    inline bool PickQueueFamilies(RendererBackendData* data)
    {
        const auto queueFamilies = data->physicalDevice.getQueueFamilyProperties();

        data->graphicsQueueIndex = UINT32_MAX;
        data->presentQueueIndex = UINT32_MAX;
        data->computeQueueIndex = UINT32_MAX;
        data->transferQueueIndex = UINT32_MAX;

        int i = 0;
        for (auto family : queueFamilies)
        {
            Log::Trace("Queue Family {0}:", i);
            Log::Trace("\tQueue Count: {0}", family.queueCount);
            Log::Trace("\tQueue Flags: {0}", vk::to_string(family.queueFlags));
            i++;
        }

        i = 0;
        for (auto family : queueFamilies)
        {
            if (family.queueFlags & vk::QueueFlagBits::eGraphics && data->graphicsQueueIndex == UINT32_MAX)
                data->graphicsQueueIndex = i;
            if (family.queueFlags & vk::QueueFlagBits::eCompute)
                data->computeQueueIndex = i;
            if (family.queueFlags & vk::QueueFlagBits::eTransfer)
                data->transferQueueIndex = i;
            if (data->physicalDevice.getSurfaceSupportKHR(i, Window::Current()->GetRenderData().surface))
                data->presentQueueIndex = i;
            
            i++;
        }

        if (data->graphicsQueueIndex != UINT32_MAX && data->presentQueueIndex != UINT32_MAX && data->computeQueueIndex != UINT32_MAX && data->transferQueueIndex != UINT32_MAX)
        {
            Log::Trace("Picked queue families:");
            Log::Trace("\tGraphics: {0}", data->graphicsQueueIndex);
            Log::Trace("\tPresent: {0}", data->presentQueueIndex);
            Log::Trace("\tCompute: {0}", data->computeQueueIndex);
            Log::Trace("\tTransfer: {0}", data->transferQueueIndex);
            return true;
        }
        
        return false;
    }

    inline bool CreateDevice(RendererBackendData* data)
    {
        constexpr float priorities = 1.0f;
        List<vk::DeviceQueueCreateInfo> queues;
        vk::DeviceQueueCreateInfo graphicsQueueInfo;
        graphicsQueueInfo.queueFamilyIndex = data->graphicsQueueIndex;
        graphicsQueueInfo.queueCount = 1;
        graphicsQueueInfo.pQueuePriorities = &priorities;
        queues.push_back(graphicsQueueInfo);

        vk::DeviceQueueCreateInfo presentQueueInfo = graphicsQueueInfo;
        presentQueueInfo.queueFamilyIndex = data->presentQueueIndex;
        if (data->presentQueueIndex != data->graphicsQueueIndex)
            queues.push_back(presentQueueInfo);

        vk::DeviceQueueCreateInfo computeQueueInfo = graphicsQueueInfo;
        computeQueueInfo.queueFamilyIndex = data->computeQueueIndex;
        if (data->graphicsQueueIndex != data->computeQueueIndex
            && data->presentQueueIndex != data->computeQueueIndex)
            queues.push_back(computeQueueInfo);
        
        vk::DeviceQueueCreateInfo transferQueueInfo = graphicsQueueInfo;
        transferQueueInfo.queueFamilyIndex = data->transferQueueIndex;
        if (data->graphicsQueueIndex != data->transferQueueIndex
            && data->presentQueueIndex != data->transferQueueIndex
            && data->computeQueueIndex != data->transferQueueIndex)
            queues.push_back(transferQueueInfo);

        vk::PhysicalDeviceFeatures deviceFeatures{};
        // Set device features here when required

        List<const char*> requiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        List<const char*> requiredLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        vk::DeviceCreateInfo createInfo{};
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queues.size());
        createInfo.pQueueCreateInfos = queues.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        Log::Info("Creating device...");
        data->device = data->physicalDevice.createDevice(createInfo);

        Log::Info("Retrieving queue handles...");
        data->graphicsQueue = data->device.getQueue(data->graphicsQueueIndex, 0);
        data->presentQueue = data->device.getQueue(data->presentQueueIndex, 0);
        data->computeQueue = data->device.getQueue(data->computeQueueIndex, 0);
        data->transferQueue = data->device.getQueue(data->transferQueueIndex, 0);
        
        return false;
    }

    inline vk::SurfaceKHR AcquireSurface(const RendererBackendData* data, Window* window)
    {
        vk::SurfaceKHR surface;

        Log::Info("Acquiring surface...");
        #ifdef FE_WINDOWS
        vk::Win32SurfaceCreateInfoKHR createInfo{};
        createInfo.hwnd = static_cast<HWND>(Window::Current()->GetPlatformHandle()->Handle);
        createInfo.hinstance = GetModuleHandle(nullptr);
        surface = data->instance.createWin32SurfaceKHR(createInfo);
        #endif

        window->GetRenderData().surface = surface;
        
        return surface;
    }
    
    inline vk::PhysicalDevice PickPhysicalDevice(RendererBackendData* data)
    {
        const auto devices = data->instance.enumeratePhysicalDevices();
        
        vk::PhysicalDevice selectedDevice;
        uint32_t currentScore = 0;

        List<const char*> requiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        
        Log::Info("Physical GPUs detected:");
        for (auto device : devices)
        {
            auto properties = device.getProperties();
            const auto& limits = properties.limits;
                
            Log::Info("\t{0}", properties.deviceName);

            Set<std::string> requiredExtensionsSet(requiredExtensions.begin(), requiredExtensions.end());

            for (vk::ExtensionProperties extension : device.enumerateDeviceExtensionProperties())
                requiredExtensionsSet.erase(extension.extensionName);
            
            uint32_t score = 10;
            if (!requiredExtensionsSet.empty())
                continue;

            if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                score += 5000;

            score += limits.maxImageDimension2D;

            if(score > currentScore)
            {
                currentScore = score;
                selectedDevice = device;
            }
        }

        FE_ASSERT(selectedDevice, "No suitable GPU found!");
        Log::Info("Picked {0} as main rendering GPU", selectedDevice.getProperties().deviceName);
        
        return selectedDevice;
    }   
}

#endif
