#include "fepch.h"
#include "Framebuffer.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanFrameBuffer.h"

namespace FusionEngine
{
    Shared<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeShared<VulkanFrameBuffer>(spec);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
