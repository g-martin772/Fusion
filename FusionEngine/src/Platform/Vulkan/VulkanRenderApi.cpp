#include "fepch.h"
#include "VulkanRenderApi.h"

#include "Core/Application.h"
#include "GLFW/glfw3.h"

// https://www.youtube.com/@GetIntoGameDev

namespace FusionEngine
{
    void VulkanRenderApi::Init()
    {
        FE_INFO("Initializing Vulkan");
        CreateInstance();

        m_DynamicInstanceDispatcher = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
        CreateDebugMessenger();

        CreateSurface();

        CreatePhysicalDevice();
        CreateLogicalDevice();
        CreateGraphicsQueue();
        CreatePresentQueue();
    }

    void VulkanRenderApi::OnWindowResize(uint32_t width, uint32_t height)
    {
    }

    void VulkanRenderApi::ShutDown()
    {
        m_LogicalDevice.destroy();

        m_Instance.destroySurfaceKHR(m_Surface);
        m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DynamicInstanceDispatcher);
        //m_Instance.destroy(); //throws for whatever reason
    }

    void VulkanRenderApi::CreateInstance()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap4.html#initialization-instances
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap44.html#extendingvulkan-coreversions-versionnumbers
        uint32_t version;
        vkEnumerateInstanceVersion(&version);
        FE_INFO("Vulkan Version: {0}.{1}.{2}", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

        vk::ApplicationInfo appInfo(
            "Fusion Engine",
            version,
            "Fusion Engine",
            version,
            version);

        // Get GLFW Window Extensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        extensions.push_back("VK_EXT_debug_utils");
        CheckSupportedExtensions(extensions);
        CheckSupportedLayers(m_EnabledLayers);
    
        vk::InstanceCreateInfo createInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            static_cast<uint32_t>(m_EnabledLayers.size()),
            m_EnabledLayers.data(),
            static_cast<uint32_t>(extensions.size()),
            extensions.data());

        try
        {
            FE_INFO("Creating Vulkan Instance...");
            m_Instance = vk::createInstance(createInfo);
            FE_INFO("Success");
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Failed to create Vulkan Instance");
        }
    }

    void VulkanRenderApi::CheckSupportedExtensions(const std::vector<const char*>& extensions) const
    {
        const std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

        FE_TRACE("Device can support following Vulkan extensions:");
        for (vk::ExtensionProperties extension : supportedExtensions)
            FE_TRACE("\t{0}", extension.extensionName);
        
        FE_TRACE("Requesting following Vulkan extensions:");
        for (const char* extension : extensions)
            FE_TRACE("\t{0}", extension);

        for (const char* extension : extensions)
        {
            bool found = false;
            for (vk::ExtensionProperties supportedExtension : supportedExtensions)
            {
                if(strcmp(extension, supportedExtension.extensionName) == 0)
                    found = true;
            }
            FE_ASSERT(found, "Vulkan Extension '{0}' is not supported by this device", extension);
        }
    }

    void VulkanRenderApi::CheckSupportedLayers(const std::vector<const char*>& layers) const
    {
        const std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

        FE_TRACE("Device can support following Vulkan layers:");
        for (vk::LayerProperties layer : supportedLayers)
            FE_TRACE("\t{0}", layer.layerName);
        
        FE_TRACE("Requesting following Vulkan layers:");
        for (const char* layer : layers)
            FE_TRACE("\t{0}", layer);

        for (const char* layer : layers)
        {
            bool found = false;
            for (vk::LayerProperties supportedLayer : supportedLayers)
            {
                if(strcmp(layer, supportedLayer.layerName) == 0)
                    found = true;
            }
            FE_ASSERT(found, "Requested Vulkan Layer is not supported by this device");
        }
    }

    // callback for https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md 
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        switch (messageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: FE_TRACE("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: FE_INFO("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: FE_WARN("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: FE_ERROR("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT: FE_FATAL("[Vulkan]: {0}", pCallbackData->pMessage); break;
            default: FE_ASSERT(false, "What error to hell is this?"); break;
        }
        return VK_FALSE;
    }
    
    void VulkanRenderApi::CreateDebugMessenger()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap49.html#debugging-debug-messengers
        vk::DebugUtilsMessengerCreateInfoEXT createInfo(
            vk::DebugUtilsMessengerCreateFlagsEXT(),
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
            debugCallback,
            nullptr);

        m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(createInfo, nullptr, m_DynamicInstanceDispatcher);
    }

    void VulkanRenderApi::CreateSurface()
    {
        VkSurfaceKHR cSurface;
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());
        if(glfwCreateWindowSurface(m_Instance, window, nullptr, &cSurface) != VK_SUCCESS)
            FE_ASSERT(false, "Failed to create Vulkan Surface")
        else
            FE_INFO("Sucessfully created Vulkan surface");

        m_Surface = cSurface;
    }

    void VulkanRenderApi::CreatePhysicalDevice()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#devsandqueues-devices

        const std::vector<vk::PhysicalDevice> availableDevices = m_Instance.enumeratePhysicalDevices();
        FE_INFO("There were {0} physical GPUs found by the engine", availableDevices.size());
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
    }

    void VulkanRenderApi::CreateLogicalDevice()
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

            if (m_PhysicalDevice.getSurfaceSupportKHR(i, m_Surface))
            {
                m_PresentFamily = i;
                FE_INFO("Queue Family {0} is suitable for presenting", i);
            }

            if (m_GraphicsFamily.has_value() && m_PresentFamily.has_value())
                break;
        }

        FE_ASSERT(m_GraphicsFamily.has_value(), "Device does not support a praphics queue");
        FE_ASSERT(m_PresentFamily.has_value(), "Device does not support a present queue");

        FE_INFO("Creating Logical Device...");
        float queuePriority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queueInfos;
        
        queueInfos.push_back(vk::DeviceQueueCreateInfo(
            vk::DeviceQueueCreateFlags(),
            m_GraphicsFamily.value(),
            1,
            &queuePriority));

        if (m_GraphicsFamily.value() != m_PresentFamily.value())
            queueInfos.push_back(vk::DeviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                m_PresentFamily.value(),
                1,
                &queuePriority));

        vk::PhysicalDeviceFeatures deviceFeatures;
        //request device features here

        vk::DeviceCreateInfo deviceInfo(
            vk::DeviceCreateFlags(),
            static_cast<uint32_t>(queueInfos.size()),
            queueInfos.data(),
            static_cast<uint32_t>(m_EnabledLayers.size()),
            m_EnabledLayers.data(),
            0,
            nullptr,
            &deviceFeatures);

        try
        {
            m_LogicalDevice = m_PhysicalDevice.createDevice(deviceInfo);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating logical device failed");
        }
        FE_INFO("Success");
    }

    void VulkanRenderApi::CreateGraphicsQueue()
    {
        FE_INFO("Creating Graphics Queue...");
        m_GraphicsQueue = m_LogicalDevice.getQueue(m_GraphicsFamily.value(), 0);
        FE_INFO("Success");
    }

    void VulkanRenderApi::CreatePresentQueue()
    {
        FE_INFO("Creating Present Queue...");
        m_GraphicsQueue = m_LogicalDevice.getQueue(m_PresentFamily.value(), 0);
        FE_INFO("Success");
    }
}
