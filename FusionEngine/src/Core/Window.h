#pragma once
#include "LayerStack.h"
#include "Time.h"

namespace FusionEngine
{
    class Window
    {
    public:
        static Shared<Window> Create();
        virtual ~Window() = default;    
    
        void Init();
        void OnUpdate();
        void ShutDown();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        
        virtual void* GetNativeWindow() = 0;

    protected:
        Window() = default;
        virtual void InitPlatform() = 0;
        virtual void OnUpdatePlatform() = 0;
        virtual void ShutDownPlatform() = 0;
    protected:
        Time m_Time;
        LayerStack m_LayerStack;
    };
}
