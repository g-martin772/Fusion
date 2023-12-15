#include "fepch.h"
#include "VertexBuffer.h"

#include "Pipeline.h"
#include "RenderApi.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"

namespace FusionEngine
{
    Shared<VertexBuffer> VertexBuffer::Create(const std::vector<Attribute>& attributes, uint32_t size)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeShared<VulkanVertexBuffer>(attributes, size);
        }
        FE_ASSERT(false, "Invalid Render API");
        return nullptr;
    }
}
