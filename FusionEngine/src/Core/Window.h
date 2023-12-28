#pragma once
#include "Time.h"
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
        void ShutDown() const;

        [[nodiscard]] WindowHandle GetPlatformHandle() const { return m_PlatformHandle; }
    private:
        WindowHandle m_PlatformHandle;
    };
}
