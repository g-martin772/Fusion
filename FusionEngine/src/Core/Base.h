#pragma once

#ifdef FUSION_ENGINE
    #define FE_API //__declspec(dllexport)
#else
    #define FE_API //__declspec(dllimport)
#endif

namespace FusionEngine
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T>
    using Unique = std::unique_ptr<T>;
    
    template<typename T>
    using Weak = std::weak_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeUnique(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}


