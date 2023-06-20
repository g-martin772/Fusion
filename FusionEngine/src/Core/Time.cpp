#include "fepch.h"
#include "Time.h"

namespace FusionEngine
{
    void Time::Update()
    {
        static std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
        const std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

        const std::chrono::duration<double> deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - previousTime);

        previousTime = currentTime;
        m_DeltaTime = deltaTime.count();
    }
}
