#include "fepch.h"

#include "VulkanDevice.h"
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
        s_Data->physicalDevice = VulkanHelpers::PickPhysicalDevice(s_Data);
        VulkanHelpers::AcquireSurface(s_Data, Application::Get()->GetCurrentWindow());
        VulkanHelpers::PickQueueFamilies(s_Data);
        VulkanHelpers::CreateDevice(s_Data);
    }

    void RendererBackend::Shutdown()
    {
        s_Data->device.destroy();
        s_Data->instance.destroySurfaceKHR(Application::Get()->GetPrimaryWindow()->GetRenderData().surface);
        s_Data->instance.destroyDebugUtilsMessengerEXT(s_Data->debugMessenger, nullptr, s_Data->instanceLoader);
        s_Data->instance.destroy();

        delete s_Data;
    }
}

#endif // FE_RENDER_BACKEND_VULKAN