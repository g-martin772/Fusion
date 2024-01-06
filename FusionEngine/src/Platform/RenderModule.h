#pragma once

#include "Core/Base.h"

namespace FusionEngine
{
    struct RenderModuleData;
    
    class RenderModule
    {
    public:
        FE_MODULE static RenderModuleData* Init();
        FE_MODULE static void Reload(RenderModuleData* data);
        FE_MODULE static void Shutdown(RenderModuleData* data);
        
        FE_MODULE static RenderModuleData* GetDataPointer();
    };
}