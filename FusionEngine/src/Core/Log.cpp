#include "fepch.h"
#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FusionEngine
{
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T][%t] %n: %v%$");

        Log::GetFusionLogger()->set_level(spdlog::level::trace);
        Log::GetForgeLogger()->set_level(spdlog::level::trace);
        Log::GetGameLogger()->set_level(spdlog::level::trace);
    }

    Shared<spdlog::logger> Log::GetFusionLogger()
    {
        static Shared<spdlog::logger> s_FusionLogger = spdlog::stdout_color_mt("FUSION");
        return s_FusionLogger;
    }

    Shared<spdlog::logger> Log::GetForgeLogger()
    {
        static Shared<spdlog::logger> s_ForgeLogger = spdlog::stdout_color_mt("FORGE");
        return s_ForgeLogger;
    }

    Shared<spdlog::logger> Log::GetGameLogger()
    {
        static Shared<spdlog::logger> s_GameLogger = spdlog::stdout_color_mt("GAME");
        return s_GameLogger;
    }
}

