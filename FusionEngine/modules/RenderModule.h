#pragma once

#if defined(FE_RENDER_MODULE)
    #define FE_MODULE __declspec(dllexport)
#else
    #define FE_MODULE __declspec(dllimport)
#endif

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