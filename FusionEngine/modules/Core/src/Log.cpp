#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FusionEngine
{
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T][%t] %n: %v%$");
    }
}

