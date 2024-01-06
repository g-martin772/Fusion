#include "Instance.h"

#include <iostream>

namespace FusionEngine
{
    Instance::Instance()
    {
        CreateInstance();
    }

    Instance::~Instance()
    {
        m_Instance.destroy();
    }

    void Instance::CreateInstance()
    {
        vk::ApplicationInfo appInfo{};
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pApplicationName = "FusionEngine";
        appInfo.pEngineName = "FusionEngine";
        appInfo.pNext = nullptr;
        
        vk::InstanceCreateInfo createInfo{};
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = 0;
        createInfo.ppEnabledExtensionNames = nullptr;

        m_Instance = vk::createInstance(createInfo);
        std::cout << "Created Vulkan instance\n";
    }
}
