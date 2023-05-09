#include "fepch.h"
#include "Log.h"

namespace FusionEngine
{
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        Log::GetFusionLogger()->set_level(spdlog::level::trace);
        Log::GetForgeLogger()->set_level(spdlog::level::trace);
        Log::GetGameLogger()->set_level(spdlog::level::trace);
    }
}

