#pragma once

#ifdef FE_RENDER_BACKEND_VULKAN

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    struct RendererBackendData
    {
        vk::Instance instance;
    };

    struct RendererBackendWindowData
    {
        
    };
}

#endif 