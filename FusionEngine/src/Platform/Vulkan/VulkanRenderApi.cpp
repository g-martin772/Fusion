#include "fepch.h"
#include "VulkanRenderApi.h"

#include "GLFW/glfw3.h"

namespace FusionEngine
{
    void VulkanRenderApi::Init()
    {
        FE_INFO("Initializing Vulkan");
        CreateInstance();

        m_DynamicInstanceDispatcher = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
        CreateDebugMessenger();
    }

    void VulkanRenderApi::OnWindowResize(uint32_t width, uint32_t height)
    {
    }

    void VulkanRenderApi::ShutDown()
    {
        m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DynamicInstanceDispatcher);
        m_Instance.destroy();
    }

    void VulkanRenderApi::CreateInstance()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap4.html#initialization-instances
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap44.html#extendingvulkan-coreversions-versionnumbers
        uint32_t version;
        vk:vkEnumerateInstanceVersion(&version);
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
        
        std::vector<const char*> layers;
        layers.push_back("VK_LAYER_KHRONOS_validation");
        CheckSupportedLayers(layers);
    
        vk::InstanceCreateInfo createInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            static_cast<uint32_t>(layers.size()),
            layers.data(),
            static_cast<uint32_t>(extensions.size()),
            extensions.data());

        try
        {
            FE_INFO("Creating Vulkan Instance...");
            m_Instance = vk::createInstance(createInfo);
            FE_INFO("Success");
        }
        catch (vk::SystemError error)
        {
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
}
