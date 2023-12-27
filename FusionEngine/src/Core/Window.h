#pragma once
#include "Time.h"

namespace FusionEngine
{
    class Window
    {
    public:
        static Window* Create();
        virtual ~Window() = default;    
    
        void Init();
        void OnUpdate();
        void ShutDown();

        virtual void* GetNativeWindow() = 0;
    protected:
        Window() = default;
        virtual void InitPlatform() = 0;
        virtual void OnUpdatePlatform() = 0;
        virtual void ShutDownPlatform() = 0;
    protected:
        Time m_Time;
    };
}
