#pragma once

#include <string>

namespace FusionEngine
{
    class Layer
    {
    public:
        Layer(std::string name = "Layer") : m_DebugName(std::move(name)) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}

        [[nodiscard]] const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}
