#include "fepch.h"

#include "VulkanInstance.h"
#include "VulkanRendererBackendData.h"
#include "Renderer/RenderBackend.h"

#ifdef FE_RENDER_BACKEND_VULKAN

namespace FusionEngine
{
    static RendererBackendData* s_RenderBackendData;
    
    void RendererBackend::Init()
    {
        s_RenderBackendData = new RendererBackendData();
        s_RenderBackendData->instance = VulkanHelpers::CreateInstance();
    }

    void RendererBackend::Shutdown()
    {
        s_RenderBackendData->instance.destroy();

        delete s_RenderBackendData;
    }
}

#endif // FE_RENDER_BACKEND_VULKAN