﻿#include "fepch.h"
#include "VertexBuffer.h"

#include "Pipeline.h"
#include "RenderApi.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"

namespace FusionEngine
{
    Ref<VertexBuffer> VertexBuffer::Create(const std::vector<Attribute>& attributes, DrawMode drawmode = DrawMode::Triangles)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: return MakeRef<VulkanVertexBuffer>(attributes, drawmode);
        }
    }
}
