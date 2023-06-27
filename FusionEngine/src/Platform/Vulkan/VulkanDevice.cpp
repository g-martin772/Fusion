#include "fepch.h"
#include "VulkanDevice.h"

#include "VulkanSwapChain.h"

namespace FusionEngine
{
    VulkanDevice::VulkanDevice(VulkanInstance* instance, const VulkanSwapChain* swapChain)
    {
        m_Instance = instance;

        CreatePhysicalDevice();
        CreateLogicalDevice(swapChain->GetSurface());
        CreateGraphicsQueue();
        CreatePresentQueue();
    }

    VulkanDevice::~VulkanDevice()
    {
        m_LogicalDevice.destroy();
    }

    void VulkanDevice::CreatePhysicalDevice()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#devsandqueues-devices

        const std::vector<vk::PhysicalDevice> availableDevices = m_Instance->GetInstance().enumeratePhysicalDevices();
        FE_INFO("There were {0} physical GPUs found by the engine:", availableDevices.size());
        for (vk::PhysicalDevice device : availableDevices)
            FE_INFO("\t{0}", device.getProperties().deviceName);

        std::vector<const char*> requiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        FE_TRACE("Requesting following Vulkan Device Extensions:");
        for (const char* extension : requiredExtensions)
            FE_TRACE("\t{0}", extension);

        uint32_t currentScore = 0;
        for (vk::PhysicalDevice device : availableDevices)
        {
            std::set<std::string> requiredExtensionsSet(requiredExtensions.begin(), requiredExtensions.end());

            for (vk::ExtensionProperties extension : device.enumerateDeviceExtensionProperties())
            {
                requiredExtensionsSet.erase(extension.extensionName);
            }
            
            uint32_t score = 0;
            if (!requiredExtensionsSet.empty())
               continue;

            if (device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                score += 5000;

            score += device.getProperties().limits.maxImageDimension2D; // around 1000 - 10000
            
            if(score > currentScore)
            {
                currentScore = score;
                m_PhysicalDevice = device;
            }
        }

        FE_ASSERT(m_PhysicalDevice, "No suitable GPU found!");
        FE_INFO("Picked {0} as rendering GPU", m_PhysicalDevice.getProperties().deviceName);
        // Note: Maybe log some info about the GPU?
    }

    void VulkanDevice::CreateLogicalDevice(vk::SurfaceKHR surface)
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#VkQueueFamilyProperties
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#VkQueueFlagBits
        std::vector<vk::QueueFamilyProperties> queueFamilies = m_PhysicalDevice.getQueueFamilyProperties();

        FE_INFO("Device can support {0} Queue families", queueFamilies.size());
        
        for (int i = 0; i < queueFamilies.size(); i++)
        {
            vk::QueueFamilyProperties queueFamily = queueFamilies[i];
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                m_GraphicsFamily = i;
                FE_INFO("Queue Family {0} is suitable for praphics", i);
            }

            if (m_PhysicalDevice.getSurfaceSupportKHR(i, surface))
            {
                m_PresentFamily = i;
                FE_INFO("Queue Family {0} is suitable for presenting", i);
            }

            if (m_GraphicsFamily.has_value() && m_PresentFamily.has_value())
                break;
        }

        FE_ASSERT(m_GraphicsFamily.has_value(), "Device does not support a praphics queue");
        FE_ASSERT(m_PresentFamily.has_value(), "Device does not support a present queue");

        float queuePriority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queueInfos;
        
        queueInfos.emplace_back(
            vk::DeviceQueueCreateFlags(),
            m_GraphicsFamily.value(),
            1,
            &queuePriority);

        if (m_GraphicsFamily.value() != m_PresentFamily.value())
            queueInfos.emplace_back(
                vk::DeviceQueueCreateFlags(),
                m_PresentFamily.value(),
                1,
                &queuePriority);

        const std::vector deviceExtensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        
        constexpr vk::PhysicalDeviceFeatures deviceFeatures {
            //request device features here
        };

        const vk::DeviceCreateInfo deviceInfo(
            vk::DeviceCreateFlags(),
            static_cast<uint32_t>(queueInfos.size()),
            queueInfos.data(),
            m_Instance->GetEnabledLayers().size(),
            m_Instance->GetEnabledLayers().data(),
            static_cast<uint32_t>(deviceExtensions.size()),
            deviceExtensions.data(),
            &deviceFeatures);

        try
        {
            FE_INFO("Creating Logical Device");
            m_LogicalDevice = m_PhysicalDevice.createDevice(deviceInfo);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating logical device failed");
        }
    }

    void VulkanDevice::CreateGraphicsQueue()
    {
        FE_INFO("Creating Graphics Queue...");
        m_GraphicsQueue = m_LogicalDevice.getQueue(m_GraphicsFamily.value(), 0);
        FE_INFO("Success");
    }

    void VulkanDevice::CreatePresentQueue()
    {
        FE_INFO("Creating Present Queue...");
        m_PresentQueue = m_LogicalDevice.getQueue(m_PresentFamily.value(), 0);
        FE_INFO("Success");
    }
}
