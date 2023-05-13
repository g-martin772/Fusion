#pragma once
#include <iostream>
#include <memory>

#include "Base.h"
#include "LayerStack.h"
#include "Window.h"
#include "Renderer/Pipeline.h"

namespace FusionEngine
{
    FE_API class Application
    {
    public:
        static Application* Get();
        
        Application();
        void Run();
        void Shutdown();

        void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        Ref<Window> GetWindow() { return m_Window; }
    private:
        bool m_Running = true;

        Ref<Window> m_Window;
        LayerStack m_LayerStack;
    };
}
