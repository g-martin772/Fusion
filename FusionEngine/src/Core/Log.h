﻿#pragma once

#include <spdlog/logger.h>

#include "Base.h"


namespace FusionEngine
{
    class Log
    {
    public:
        static void Init();

        static Shared<spdlog::logger> GetFusionLogger();
        static Shared<spdlog::logger> GetForgeLogger();
        static Shared<spdlog::logger> GetGameLogger();
    }; 
}

#ifdef FE_LOG_LEVEL_TRACE
#define FE_TRACE(...)	    ::FusionEngine::Log::GetFusionLogger()->trace(__VA_ARGS__)
#define FF_TRACE(...)		::FusionEngine::Log::GetForgeLogger()->trace(__VA_ARGS__)
#define FG_TRACE(...)	    ::FusionEngine::Log::GetGameLogger()->trace(__VA_ARGS__)
#else
#define FE_TRACE(...)
#define FF_TRACE(...)
#define FG_TRACE(...)
#endif

#if defined(FE_LOG_LEVEL_INFO) || defined(FE_LOG_LEVEL_TRACE)
#define FE_INFO(...)		::FusionEngine::Log::GetFusionLogger()->info(__VA_ARGS__)
#define FF_INFO(...)		::FusionEngine::Log::GetForgeLogger()->info(__VA_ARGS__)
#define FG_INFO(...)		::FusionEngine::Log::GetGameLogger()->info(__VA_ARGS__)
#else
#define FE_INFO(...)
#define FF_INFO(...)
#define FG_INFO(...)
#endif

#if defined(FE_LOG_LEVEL_WARN) || defined(FE_LOG_LEVEL_INFO) || defined(FE_LOG_LEVEL_TRACE)
#define FG_WARN(...)		::FusionEngine::Log::GetGameLogger()->warn(__VA_ARGS__)
#define FF_WARN(...)		::FusionEngine::Log::GetForgeLogger()->warn(__VA_ARGS__)
#define FE_WARN(...)		::FusionEngine::Log::GetFusionLogger()->warn(__VA_ARGS__)
#else
#define FG_WARN(...)
#define FF_WARN(...)
#define FE_WARN(...)
#endif


#define FE_FATAL(...)	    ::FusionEngine::Log::GetFusionLogger()->critical(__VA_ARGS__)
#define FE_ERROR(...)	    ::FusionEngine::Log::GetFusionLogger()->error(__VA_ARGS__)

#define FF_FATAL(...)		::FusionEngine::Log::GetForgeLogger()->critical(__VA_ARGS__)
#define FF_ERROR(...)		::FusionEngine::Log::GetForgeLogger()->error(__VA_ARGS__)

#define FG_FATAL(...)	    ::FusionEngine::Log::GetGameLogger()->critical(__VA_ARGS__)
#define FG_ERROR(...)	    ::FusionEngine::Log::GetGameLogger()->error(__VA_ARGS__)

