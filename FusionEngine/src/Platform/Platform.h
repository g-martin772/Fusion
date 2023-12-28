#pragma once

#include "Core/Result.h"

namespace FusionEngine
{
    struct PlatformState
    {
        void* InternalState;
    };

    struct WindowHandle
    {
        void* Handle;
    };

    enum class PlatformError
    {
        NotImplemented,
        RegisterWindowClassFailed,
        WindowCreationFailed
    };
    
    class Platform
    {
    public:
        static PlatformState State;
        
        static Result<PlatformState, PlatformError> Init();
        static void Update();
        static void ShutDown();

        static Result<WindowHandle, PlatformError> CreateNativeWindow();
        static void DestroyNativeWindow(WindowHandle handle);

        static double GetAbsTime();

        static void SleepM(double ms);       
    };
}
