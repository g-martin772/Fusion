#include "fepch.h"
#include "Framebuffer.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanFrameBuffer.h"

namespace FusionEngine
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeRef<VulkanFrameBuffer>(spec);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
