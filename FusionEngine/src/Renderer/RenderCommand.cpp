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

    void RenderCommand::BeginFrame()
    {
        s_RenderApi->BeginFrame();
    }

    void RenderCommand::BeginSwapchainRenderPass()
    {
        s_RenderApi->BeginSwapchainRenderPass();
    }

    void RenderCommand::EndSwapchainRenderPass()
    {
        s_RenderApi->EndSwapchainRenderPass();
    }

    void RenderCommand::Draw(uint32_t vertexCount)
    {
        s_RenderApi->Draw(vertexCount);
    }

    void RenderCommand::DrawIndexed(uint32_t indexCount)
    {
        s_RenderApi->DrawIndexed(indexCount);
    }

    void RenderCommand::EndFrame()
    {
        s_RenderApi->EndFrame();
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
