#pragma once

#include <memory>

#ifdef FUSION_ENGINE
    #define FE_API //__declspec(dllexport)
#else
    #define FE_API //__declspec(dllimport)
#endif

#define BIT(x) (1 << x)

#ifdef FE_DEBUG
    #define FE_LOG_LEVEL_TRACE 
#elif FE_RELEASE
    #define FE_LOG_LEVEL_INFO
#else
    #defineFE_LOG_LEVEL_WARN
#endif

namespace FusionEngine
{
    template<typename T>
    using Shared = std::shared_ptr<T>;
    
    template<typename T>
    using Unique = std::unique_ptr<T>;
    
    template<typename T>
    using Weak = std::weak_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Shared<T> MakeShared(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    constexpr Unique<T> MakeUnique(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
