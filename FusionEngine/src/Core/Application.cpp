#include "fepch.h"
#include "Application.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    void Application::Init()
    {
        Log::Init();
        FE_INFO("Startin Fusion Engine");
        uint32_t version;
        vk::enumerateInstanceVersion(&version);
        FE_INFO("{0}.{1}.{2}", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
    }

    void Application::Run()
    {
        while(m_Running)
        {
            
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Shutdown");
    }
}

