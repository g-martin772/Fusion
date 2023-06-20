#pragma once
#include <chrono>

namespace FusionEngine
{
    struct TimeStep
    {
        double Milliseconds;
        TimeStep(const double milliseconds = 0.0) : Milliseconds(milliseconds) {}
        operator double() const { return Milliseconds; }
        operator float() const { return static_cast<float>(Milliseconds); }
    };
    
    struct Time
    {
        void Update();
        TimeStep GetDeltaTime() const { return m_DeltaTime; }
    private:
        std::chrono::high_resolution_clock::time_point m_StartTime, m_CurrentTime;
        TimeStep m_DeltaTime;
    };
}
