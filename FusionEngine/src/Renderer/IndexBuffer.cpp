#include "fepch.h"
#include "IndexBuffer.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

namespace FusionEngine
{
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeRef<VulkanIndexBuffer>(size);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
