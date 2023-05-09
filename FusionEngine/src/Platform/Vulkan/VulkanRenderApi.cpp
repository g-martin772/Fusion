#include "fepch.h"
#include "VulkanRenderApi.h"

#include "GLFW/glfw3.h"

void VulkanRenderApi::Init()
{
    FE_INFO("Initializing Vulkan");
    CreateInstance();
}

void VulkanRenderApi::OnWindowResize(uint32_t width, uint32_t height)
{
}

void VulkanRenderApi::ShutDown()
{
    m_Instance.destroy();
}

void VulkanRenderApi::CreateInstance()
{
    // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap4.html#initialization-instances
    // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap44.html#extendingvulkan-coreversions-versionnumbers
    uint32_t version;
    vk:vkEnumerateInstanceVersion(&version);

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
    #ifdef FE_DEBUG
    FE_INFO("Requesting following Vulkan extensions:");
    for (const char * extension : extensions)
        FE_INFO("\t{0}", extension);
    #endif
    
    
    vk::InstanceCreateInfo createInfo(
        vk::InstanceCreateFlags(),
        &appInfo,
        0,
        nullptr,
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
