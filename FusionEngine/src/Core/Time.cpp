#include "fepch.h"
#include "Time.h"

namespace FusionEngine
{
    Time::Time(): m_DeltaTime(0), m_Fps(0)
    {
        m_LastSecond = 0.0;
    }

    void Time::OnUpdate()
    {
        static std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
        const std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

        const std::chrono::duration<double> deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - previousTime);

        previousTime = currentTime;
        m_DeltaTime = deltaTime.count();

        m_LastSecond += m_DeltaTime;
        m_FrameCount++;
        
        if(m_LastSecond >= 1.0)
        {
            m_Fps = m_FrameCount;
            FE_TRACE("Currently running at {0} fps", m_Fps);
            m_FrameCount = 0;
            m_LastSecond = 0.0;
        }
    }
}
