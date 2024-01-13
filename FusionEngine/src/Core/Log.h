#pragma once

#include <spdlog/logger.h>

#include "Base.h"

namespace FusionEngine
{
    class Log
    {
    public:
        static void Init();

        template <typename ... Params>
        static void Trace(spdlog::format_string_t<Params...> message, Params &&... params)
        {
            logger->trace(message, std::forward<Params>(params)...);
            fileLogger->trace(message, std::forward<Params>(params)...);
        }

        template <typename ...Params>
        static void Debug(spdlog::format_string_t<Params...> message, Params &&... params)
        {
            logger->debug(message, std::forward<Params>(params)...);
            fileLogger->debug(message, std::forward<Params>(params)...);
        }

        template <typename ... Params>
        static void Info(spdlog::format_string_t<Params...> message, Params &&... params)
        {
        
            logger->info(message, std::forward<Params>(params)...);
            fileLogger->info(message, std::forward<Params>(params)...);
        }

        template <typename ... Params>
        static void Warn(spdlog::format_string_t<Params...> message, Params &&... params)
        {
            logger->warn(message, std::forward<Params>(params)...);
            fileLogger->warn(message, std::forward<Params>(params)...);
        }

        template <typename ... Params>
        static void Error(spdlog::format_string_t<Params...> message, Params &&... params)
        {
            logger->error(message, std::forward<Params>(params)...);
            fileLogger->error(message, std::forward<Params>(params)...);
        }

        template <typename ... Params>
        static void Fatal(spdlog::format_string_t<Params...> message, Params &&... params)
        {
            logger->error(message, std::forward<Params>(params)...);
            fileLogger->error(message, std::forward<Params>(params)...);
        }

        static std::shared_ptr<spdlog::logger> logger;
        static std::shared_ptr<spdlog::logger> fileLogger;
    }; 
}
