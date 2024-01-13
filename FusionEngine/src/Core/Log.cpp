#include "fepch.h"
#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace FusionEngine
{
    std::shared_ptr<spdlog::logger> Log::logger;
    std::shared_ptr<spdlog::logger> Log::fileLogger;
    
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T][%t] %n: %v%$");
        logger = spdlog::stdout_color_mt("FusionEngine");
        fileLogger = spdlog::basic_logger_mt("FusionLog", "logs/latest.log");
    }

    
}

