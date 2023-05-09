#pragma once
#include "Renderer/RenderApi.h"

#include <vulkan/vulkan.hpp>

class VulkanRenderApi : public FusionEngine::RenderApi
{
public:
    void Init() override;
    void OnWindowResize(uint32_t width, uint32_t height) override;
    void ShutDown() override;
private:
    void CreateInstance();
private:
    vk::Instance m_Instance;
};
