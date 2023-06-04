#pragma once
#include <chrono>

namespace FusionEngine
{
    struct Time
    {
        Time();
        ~Time() = default;

        void OnUpdate();

        uint32_t GetFps() const { return m_Fps; }
        double GetDeltaTime() const { return m_DeltaTime; }
    
    private:
        std::chrono::high_resolution_clock::time_point m_StartTime, m_CurrentTime;
        double m_DeltaTime, m_LastSecond;
        uint32_t m_Fps, m_FrameCount;
    };
}
