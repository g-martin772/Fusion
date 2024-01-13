#pragma once

#include <glm/vec2.hpp>

#include "Core/Result.h"
#include "Core/Window/KeyCodes.h"
#include "Renderer/RenderBackend.h"
#include "InputMap.h"

namespace FusionEngine
{
    class Window;
    class InputMap;

    struct PlatformState
    {
        void* InternalState;
    };

    struct WindowHandle
    {
        void* Handle;
        Unique<InputMap> InputHandle;
        Unique<RendererBackendWindowData> RendererData;
    };

    enum class PlatformError
    {
        NotImplemented,
        RegisterWindowClassFailed,
        WindowCreationFailed,
        NoDisplayFound,
    };
    
    class Platform
    {
    public:
        static PlatformState State;
        
        static Result<PlatformState, PlatformError> Init();
        static void Update();
        static void ShutDown();

        static Result<WindowHandle, PlatformError> CreateNativeWindow(Window* instance);
        static void UpdateNativeWindow(WindowHandle& handle);
        static void DestroyNativeWindow(WindowHandle& handle);

        static double GetAbsTime();

        static void SleepM(double ms);
        
        static bool IsKeyDown(KeyCode keycode);
        static bool IsButtonDown(MouseCode button);
        static glm::uvec2 GetMouse();
        static glm::uvec2 GetMouseDelta();
    };
}
