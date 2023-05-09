#pragma once
#include "Renderer/RenderApi.h"

class VulkanRenderApi : public FusionEngine::RenderApi
{
public:
    void Init() override;
    void OnWindowResize(uint32_t width, uint32_t height) override;
    void ShutDown() override;
};
