#pragma once
#include <glm/vec2.hpp>

#include "KeyCodes.h"
#include "Platform/Platform.h"

namespace FusionEngine
{
    namespace Input
    {
        inline bool IsKeyDown(const KeyCode keycode) { return Platform::IsKeyDown(keycode); }
        inline bool IsButtonDown(const MouseCode button) { return Platform::IsButtonDown(button); }
        inline glm::vec2 GetMouse() { return Platform::GetMouse(); }
        inline glm::vec2 GetMouseDelta() { return Platform::GetMouseDelta(); }
 
        // inline void CaptureMouse(bool capture) { s_Instance->CaptureMouseImpl(capture); }
    };
}
