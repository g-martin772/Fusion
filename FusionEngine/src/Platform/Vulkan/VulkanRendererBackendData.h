#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    struct RendererBackendData
    {
        vk::Instance instance;
        vk::DispatchLoaderDynamic instanceLoader;
        vk::DebugUtilsMessengerEXT debugMessenger;
    };

    struct RendererBackendWindowData
    {
        
    };
}

#endif 