#pragma once
#include "Platform/Platform.h"

namespace FusionEngine
{
    class Window final
    {
    public:
        static Shared<Window> Create();
        Window();
        ~Window() = default;    

        void Init();
        void OnUpdate();
        void ShutDown();

        [[nodiscard]] WindowHandle GetPlatformHandle() const { return m_PlatformHandle; }
    private:
        WindowHandle m_PlatformHandle;
    };
}
