#pragma once

#include <string>

namespace FusionEngine
{
    class Layer
    {
    public:
        explicit Layer(std::string name = "Layer") : m_DebugName(std::move(name)) {}
        virtual ~Layer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate() = 0;

        [[nodiscard]] const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}
