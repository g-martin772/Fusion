#include "fepch.h"
#include "RenderCommand.h"

#include "Platform/Vulkan/VulkanRenderApi.h"

namespace FusionEngine
{
    Unique<RenderApi> RenderCommand::s_RenderApi = RenderApi::Create();
    
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
}
