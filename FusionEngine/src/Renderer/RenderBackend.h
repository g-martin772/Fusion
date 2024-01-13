#pragma once

#include "Platform/Vulkan/VulkanRendererBackendData.h"

namespace FusionEngine
{
    class Window;

    struct RendererBackendData;
    struct RendererBackendWindowData;
    
    class RendererBackend
    {
    public:
        static void Init();
        static void Shutdown();
    };
}
