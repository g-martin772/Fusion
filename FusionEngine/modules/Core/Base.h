#pragma once

#include <memory>
#include <utility>

#include <vector>
#include <unordered_map>
#include <map>

#ifdef FUSION_ENGINE
    #define FE_API __declspec(dllexport)
#else
    #define FE_API __declspec(dllimport)
#endif

#ifdef FUSION_CORE_MODULE
    #define FE_CORE_MODULE_API __declspec(dllexport)
#else
    #define FE_CORE_MODULE_API __declspec(dllimport)
#endif

#ifdef VULKAN_MODULE
    #define FE_VULKAN_MODULE_API __declspec(dllexport)
#else
    #define FE_VULKAN_MODULE_API __declspec(dllimport)
#endif

#define BIT(x) (1 << x)

#ifdef FE_DEBUG
    #define FE_LOG_LEVEL_TRACE 
#elif FE_RELEASE
    #define FE_LOG_LEVEL_INFO
#else
    #define FE_LOG_LEVEL_WARN
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

    template<typename T>
    using List = std::vector<T>;

    template<typename T, typename ... Args>
    constexpr List<T> MakeList(Args&& ... args)
    {
        return std::vector<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename U>
    using UMap = std::unordered_map<T, U>;

    template<typename T, typename U, typename ... Args>
    constexpr UMap<T, U> MakeUMap(Args&& ... args)
    {
        return std::unordered_map<T, U>(std::forward<Args>(args)...);
    }

    template<typename T, typename U>
    using Map = std::unordered_map<T, U>;

    template<typename T, typename U, typename ... Args>
    constexpr Map<T, U> MakeMap(Args&& ... args)
    {
        return std::map<T, U>(std::forward<Args>(args)...);
    }
}
