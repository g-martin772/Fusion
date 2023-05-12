#include "fepch.h"
#include "Pipeline.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanPipeline.h"

namespace FusionEngine
{
    Ref<Pipeline> Pipeline::Create(const Ref<Shader>& shader)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeRef<VulkanPipeline>(shader);
        }

        FE_ASSERT(false, "No Graphics API selected");
        return nullptr;
    }
}
