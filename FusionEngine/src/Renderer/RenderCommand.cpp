#include "fepch.h"
#include "RenderCommand.h"

#include "Platform/Vulkan/VulkanRenderApi.h"

namespace FusionEngine
{
    Ref<RenderApi> RenderCommand::s_RenderApi = RenderApi::Create();
    
    void RenderCommand::Init()
    {
        s_RenderApi->Init();
    }

    void RenderCommand::ShutDown()
    {
        s_RenderApi->ShutDown();
    }

    void RenderCommand::Render()
    {
        s_RenderApi->Render();
    }

    void RenderCommand::ResizeWindow(uint32_t width, uint32_t height)
    {
        s_RenderApi->OnWindowResize(width, height);
    }

    const Ref<RenderApi>& RenderCommand::GetRenderApi()
    {
        return s_RenderApi;
    }
}
