#pragma once
#include "Renderer/RenderApi.h"

// https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md
#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi : public RenderApi
    {
    public:
        void Init() override;
        void OnWindowResize(uint32_t width, uint32_t height) override;
        void ShutDown() override;
    private:
        void CreateInstance();
        void CheckSupportedExtensions(const std::vector<const char*>& extensions) const;
        void CheckSupportedLayers(const std::vector<const char*>& layers) const;
        void CreateDebugMessenger();

        void CreatePhysicalDevice();
    private:
        vk::Instance m_Instance;
        vk::DispatchLoaderDynamic m_DynamicInstanceDispatcher;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;

        vk::PhysicalDevice m_PhysicalDevice;
    };

}
