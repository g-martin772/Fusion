#pragma once
#include "Vulkan/Instance.h"

namespace FusionEngine
{
    struct RenderModuleData
    {
        Instance instance;
        
        void Reset()
        {
            instance = Instance();
        }
    };
}
