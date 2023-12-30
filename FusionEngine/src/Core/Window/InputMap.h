#pragma once

#include <glm/vec2.hpp>

#include "KeyCodes.h"

namespace FusionEngine
{
    enum class KeyState
    {
        Up,
        Down,
        Pressed,
        Released
    };
    
    class InputMap
    {
    public:
        void UpdateKeyState(KeyCode key, KeyState state)
        {
            m_KeyStates[key] = state;
        }

        void UpdateButtonState(MouseCode button, KeyState state)
        {
            m_MouseButtonStates[button] = state;
        }

        void UpdateMousePosition(const glm::uvec2& pos)
        {
            m_PreviousMousePos = m_CurrentMousePos;
            m_CurrentMousePos = pos;
        }

        void UpdateScrollPosition(uint32_t pos)
        {
            m_PreviousScrollPos = m_CurrentScrollPos;
            m_CurrentScrollPos = pos;
        }

        [[nodiscard]] KeyState GetKeyState(KeyCode key) const
        {
            auto it = m_KeyStates.find(key);
            return it != m_KeyStates.end() ? it->second : KeyState::Up;
        }

        [[nodiscard]] KeyState GetButtonState(MouseCode button) const
        {
            auto it = m_MouseButtonStates.find(button);
            return it != m_MouseButtonStates.end() ? it->second : KeyState::Up;
        }

        [[nodiscard]] glm::uvec2 GetMouseDelta() const
        {
            return m_CurrentMousePos - m_PreviousMousePos;
        }

        [[nodiscard]] glm::uvec2 GetMouse() const { return m_CurrentMousePos; }

        [[nodiscard]] uint32_t GetScrollDelta() const
        {
            return m_CurrentScrollPos - m_PreviousScrollPos;
        }
        
        [[nodiscard]] uint32_t GetScroll() const { return m_CurrentScrollPos; }
    private:
        UMap<KeyCode, KeyState> m_KeyStates;
        UMap<MouseCode, KeyState> m_MouseButtonStates;
        glm::uvec2 m_CurrentMousePos = {0.0f, 0.0f};
        glm::uvec2 m_PreviousMousePos = {};
        uint32_t m_CurrentScrollPos = 0;
        uint32_t m_PreviousScrollPos = 0;
    };
}
