#pragma once

#include "Base.h"

namespace FusionEngine
{
    FE_CORE_MODULE_API class Log
    {
    public:
        FE_CORE_MODULE_API static void Init();
    }; 
}

#define FE_TRACE(...)
#define FE_INFO(...)
#define FE_WARN(...)
#define FE_ERROR(...)
#define FE_FATAL(...)