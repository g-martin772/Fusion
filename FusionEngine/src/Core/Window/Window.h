#pragma once
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

        [[nodiscard]] WindowHandle GetPlatformHandle() const { return m_PlatformHandle; }
    private:
        WindowHandle m_PlatformHandle;
    };
}
