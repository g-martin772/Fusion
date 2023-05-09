#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

#include "Base.h"

namespace FusionEngine
{
    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger> GetFusionLogger() {
            static Ref<spdlog::logger> s_FusionLogger = spdlog::stdout_color_mt("FUSION");
            return s_FusionLogger;
        }

        static Ref<spdlog::logger> GetForgeLogger() {
            static Ref<spdlog::logger> s_ForgeLogger = spdlog::stdout_color_mt("FORGE");
            return s_ForgeLogger;
        }

        static Ref<spdlog::logger> GetGameLogger() {
            static Ref<spdlog::logger> s_GameLogger = spdlog::stdout_color_mt("GAME");
            return s_GameLogger;
        }
    }; 
}

#define FE_FATAL(...)	    ::FusionEngine::Log::GetFusionLogger()->critical(__VA_ARGS__)
#define FE_ERROR(...)	    ::FusionEngine::Log::GetFusionLogger()->error(__VA_ARGS__)
#define FE_WARN(...)		::FusionEngine::Log::GetFusionLogger()->warn(__VA_ARGS__)
#define FE_INFO(...)		::FusionEngine::Log::GetFusionLogger()->info(__VA_ARGS__)
#define FE_TRACE(...)	    ::FusionEngine::Log::GetFusionLogger()->trace(__VA_ARGS__)

#define FF_FATAL(...)		::FusionEngine::Log::GetForgeLogger()->critical(__VA_ARGS__)
#define FF_ERROR(...)		::FusionEngine::Log::GetForgeLogger()->error(__VA_ARGS__)
#define FF_WARN(...)		::FusionEngine::Log::GetForgeLogger()->warn(__VA_ARGS__)
#define FF_INFO(...)		::FusionEngine::Log::GetForgeLogger()->info(__VA_ARGS__)
#define FF_TRACE(...)		::FusionEngine::Log::GetForgeLogger()->trace(__VA_ARGS__)

#define FG_FATAL(...)	    ::FusionEngine::Log::GetGameLogger()->critical(__VA_ARGS__)
#define FG_ERROR(...)	    ::FusionEngine::Log::GetGameLogger()->error(__VA_ARGS__)
#define FG_WARN(...)		::FusionEngine::Log::GetGameLogger()->warn(__VA_ARGS__)
#define FG_INFO(...)		::FusionEngine::Log::GetGameLogger()->info(__VA_ARGS__)
#define FG_TRACE(...)	    ::FusionEngine::Log::GetGameLogger()->trace(__VA_ARGS__)

