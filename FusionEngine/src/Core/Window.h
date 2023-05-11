#pragma once

namespace FusionEngine
{
    class Window
    {
    public:
        static Ref<Window> Create();
    
        virtual void Init() = 0;
        virtual void OnUpdate() = 0;
        virtual void ShutDown() = 0;
        virtual void* GetNativeWindow() = 0;
    };
}
