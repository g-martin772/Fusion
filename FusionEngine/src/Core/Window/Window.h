#pragma once
#include "LayerStack.h"
#include "Platform/Platform.h"

namespace FusionEngine
{
    class Window final
    {
    public:
        static Window* Create();
        Window();
        ~Window() = default;    

        void Init();
        void OnUpdate();
        void ShutDown();

        void MakeCurrent();
        static Window* Current();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        [[nodiscard]] LayerStack& GetLayerStack() { return m_LayerStack; }

        [[nodiscard]] WindowHandle GetPlatformHandle() const { return m_PlatformHandle; }
    private:
        WindowHandle m_PlatformHandle;
        LayerStack m_LayerStack;
    };
}
