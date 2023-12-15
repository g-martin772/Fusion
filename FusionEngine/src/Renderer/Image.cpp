#include "fepch.h"
#include "Image.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanImage.h"

namespace FusionEngine
{
    Shared<Image> Image::Create(const ImageSpecification& spec)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeShared<VulkanImage>(spec);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
