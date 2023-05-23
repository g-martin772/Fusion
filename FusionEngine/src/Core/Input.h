#pragma once
#include <glm/vec2.hpp>

#include "KeyCodes.h"

namespace FusionEngine
{
    class Input
    {
    public:
        Input() = default;
        virtual ~Input() = default;
        
        static void Init();
        static void Shutdown();
        
        static bool IsKeyPressed(const KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        static bool IsMouseButtonPressed(const MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

        virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual glm::vec2 GetMousePositionImpl()= 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
    private:
        static Input* s_Instance;
    };
}
