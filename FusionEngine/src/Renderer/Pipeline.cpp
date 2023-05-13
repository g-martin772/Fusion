#include "fepch.h"
#include "Pipeline.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanPipeline.h"

namespace FusionEngine
{
    Ref<Pipeline> Pipeline::Create(const Pipeline::PipelineSpecification& specification)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeRef<VulkanPipeline>(specification);
        }

        FE_ASSERT(false, "No Graphics API selected");
        return nullptr;
    }
}
