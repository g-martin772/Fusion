#pragma once

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanInstance
    {
    public:
        VulkanInstance();
        ~VulkanInstance();

        [[nodiscard]] vk::Instance GetInstance() const { return m_Instance; }
        [[nodiscard]] vk::DispatchLoaderDynamic GetDynamicInstanceDispatcher() const { return m_DynamicInstanceDispatcher; }
        [[nodiscard]] vk::DebugUtilsMessengerEXT GetDebugMessenger() const { return m_DebugMessenger; }
        [[nodiscard]] std::vector<const char*>& GetEnabledLayers() { return m_EnabledLayers; }
    private:
        void CheckSupportedExtensions(const std::vector<const char*>& extensions) const;
        void CheckSupportedLayers(const std::vector<const char*>& layers) const;
        void CreateDebugMessenger();

        vk::Instance m_Instance;
        vk::DispatchLoaderDynamic m_DynamicInstanceDispatcher;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;
        std::vector<const char*> m_EnabledLayers {
            #ifdef FE_DEBUG
                "VK_LAYER_KHRONOS_validation"
            #endif
        };
    };
}
