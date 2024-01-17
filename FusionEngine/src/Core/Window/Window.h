#pragma once
#include "LayerStack.h"
#include "Platform/Platform.h"

#ifdef FE_RENDER_BACKEND_VULKAN
#include "Platform/Vulkan/VulkanRendererBackendData.h"
#endif

namespace FusionEngine
{
    class Window final
    {
    public:
        static Unique<Window> Create(const std::string& name = "FusionEngine", glm::uvec2 size = { 1200, 800 }, glm::uvec2 pos = { 100, 100 });
        Window(std::string name, glm::uvec2 size, glm::uvec2 pos);
        ~Window();    

        void OnUpdate();
        void ShutDown();

        void MakeCurrent();
        static Window* Current();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        [[nodiscard]] glm::uvec2 GetSize() const { return m_Size; }
        [[nodiscard]] glm::uvec2 GetFrameSize() const { return m_FrameSize; }
        [[nodiscard]] glm::uvec2 GetPos() const { return m_Pos; }
        [[nodiscard]] std::string_view GetName() const { return m_Name; }

        [[nodiscard]] LayerStack& GetLayerStack() { return m_LayerStack; }

        [[nodiscard]] WindowHandle* GetPlatformHandle() { return &m_PlatformHandle; }
        [[nodiscard]] RendererBackendWindowData& GetRenderData() { return m_RenderData; }
    private:
        WindowHandle m_PlatformHandle;
        RendererBackendWindowData m_RenderData;
        LayerStack m_LayerStack;
        glm::vec2 m_Size, m_FrameSize, m_Pos;
        std::string m_Name;
        bool m_IsActive = false;

        friend class Platform;
    };
}
