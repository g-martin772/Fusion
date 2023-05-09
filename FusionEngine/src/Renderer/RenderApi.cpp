#include "fepch.h"
#include "RenderApi.h"

#include "Platform/Vulkan/VulkanRenderApi.h"

namespace FusionEngine
{
    Unique<RenderApi> RenderApi::Create()
    {
        switch (GetApi())
        {
            case Api::Vulkan:
                return MakeUnique<VulkanRenderApi>();
        }
        return nullptr;
    }
}
