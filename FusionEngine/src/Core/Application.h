#pragma once
#include <iostream>
#include <memory>

#include "Base.h"
#include "Window.h"
#include "Renderer/Pipeline.h"

namespace FusionEngine
{
    FE_API class Application
    {
    public:
        static Application* Get();
        
        void Init();
        void Run();
        void Shutdown();

        void SetRunning(const bool state) { m_Running = state; }
        Ref<Window> GetWindow() { return m_Window; }
    private:
        bool m_Running = true;

        Ref<Window> m_Window;
        Ref<Pipeline> m_PipeLine;
    };
}
