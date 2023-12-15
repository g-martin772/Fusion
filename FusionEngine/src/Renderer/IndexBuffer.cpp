#include "fepch.h"
#include "IndexBuffer.h"

#include "RenderApi.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"

namespace FusionEngine
{
    Shared<IndexBuffer> IndexBuffer::Create(uint32_t size)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeShared<VulkanIndexBuffer>(size);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
