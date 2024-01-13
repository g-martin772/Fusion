#pragma once

#include "Log.h"

#include <signal.h>

#ifdef FE_WINDOWS
    #include <Windows.h>
    #define FE_BREAK if(IsDebuggerPresent()) __debugbreak()
#else
    #include <sys/ptrace.h>
    #define FE_BREAK if(ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1) raise(SIGTRAP)
#endif

#ifdef FE_DEBUG
    #define FE_ENABLE_ASSERTS
#endif

#ifdef FE_ENABLE_ASSERTS
#define FE_ASSERT(x, ...) { if(!(x)) { Log::Error("Assertion Failed ({0}, {1}): {2}", __FILE__, __LINE__, __VA_ARGS__); FE_BREAK; } }
#define FF_ASSERT(x, ...) { if(!(x)) { Log::Error("Assertion Failed ({0}, {1}): {2}", __FILE__, __LINE__, __VA_ARGS__); FE_BREAK; } }
#define FG_ASSERT(x, ...) { if(!(x)) { Log::Error("Assertion Failed ({0}, {1}): {2}", __FILE__, __LINE__, __VA_ARGS__); FE_BREAK; } }
#else
#define FE_ASSERT(x, ...) { if(!(x)) { Log::Error("Assertion Failed ({0}, {1}): {2}", __FILE__, __LINE__, __VA_ARGS__); } }
#define FF_ASSERT(x, ...) { if(!(x)) { Log::Error("Assertion Failed ({0}, {1}): {2}", __FILE__, __LINE__, __VA_ARGS__); } }
#define FG_ASSERT(x, ...) { if(!(x)) { Log::Error("Assertion Failed ({0}, {1}): {2}", __FILE__, __LINE__, __VA_ARGS__); } }
#endif