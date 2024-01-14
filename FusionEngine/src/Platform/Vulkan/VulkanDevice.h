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
            Log::Trace("\tQueue Family {0}:", i);
            Log::Trace("\t\tQueue Count: {0}", family.queueCount);
            Log::Trace("\t\tQueue Flags: {0}", vk::to_string(family.queueFlags));
            i++;
        }

        i = 0;
        for (auto family : queueFamilies)
        {
            // Check out dedicated queue families
            if (family.queueFlags & vk::QueueFlagBits::eGraphics)
                data->graphicsQueueIndex = i;
            if (family.queueFlags & vk::QueueFlagBits::eCompute)
                data->computeQueueIndex = i;
            if (family.queueFlags & vk::QueueFlagBits::eTransfer)
                data->transferQueueIndex = i;
            if (data->physicalDevice.getSurfaceSupportKHR(i, data->surface))
                data->presentQueueIndex = i;

            if (data->graphicsQueueIndex != UINT32_MAX && data->presentQueueIndex != UINT32_MAX && data->computeQueueIndex != UINT32_MAX && data->transferQueueIndex != UINT32_MAX)
            {
                Log::Trace("Picked queue families:");
                Log::Trace("\tGraphics: {0}", data->graphicsQueueIndex);
                Log::Trace("\tPresent: {0}", data->presentQueueIndex);
                Log::Trace("\tCompute: {0}", data->computeQueueIndex);
                Log::Trace("\tTransfer: {0}", data->transferQueueIndex);
                return true;
            }
            
            i++;
        }
        
        return false;
    }

    inline vk::SurfaceKHR AcquireSurface(const RendererBackendData* data)
    {
        vk::SurfaceKHR surface;
        
        #ifdef FE_WINDOWS
        vk::Win32SurfaceCreateInfoKHR createInfo{};
        createInfo.hwnd = static_cast<HWND>(Application::Get()->GetPrimaryWindow()->GetPlatformHandle()->Handle);
        createInfo.hinstance = GetModuleHandle(nullptr);
        surface = data->instance.createWin32SurfaceKHR(createInfo);
        #endif

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
