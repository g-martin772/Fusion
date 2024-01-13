#include "fepch.h"

#include "VulkanInstance.h"
#include "VulkanRendererBackendData.h"
#include "Renderer/RenderBackend.h"

#ifdef FE_RENDER_BACKEND_VULKAN

namespace FusionEngine
{
    static RendererBackendData* s_Data;
    
    void RendererBackend::Init()
    {
        s_Data = new RendererBackendData();
        s_Data->instance = VulkanHelpers::CreateInstance();
        s_Data->instanceLoader = vk::DispatchLoaderDynamic(s_Data->instance, vkGetInstanceProcAddr);
        s_Data->instanceLoader.init(s_Data->instance);
        s_Data->debugMessenger = VulkanHelpers::CreateDebugUtilsMessengerEXT(s_Data);
    }

    void RendererBackend::Shutdown()
    {
        s_Data->instance.destroyDebugUtilsMessengerEXT(s_Data->debugMessenger, nullptr, s_Data->instanceLoader);
        s_Data->instance.destroy();

        delete s_Data;
    }
}

#endif // FE_RENDER_BACKEND_VULKAN