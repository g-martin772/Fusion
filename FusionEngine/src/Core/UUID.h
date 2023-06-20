#pragma once

// Undefine max macro from Windows.h ...
#undef max

#include <random>

namespace FusionEngine
{
    struct UUID
    {
        UUID()
        {
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

            Low = dis(gen);
            High = dis(gen);
        }

        UUID(const uint64_t low, const uint64_t high)
            : Low(low), High(high) {}

        std::string ToString() const { return std::to_string(Low) + std::to_string(High); }

        // Why the heck is there no uint128_t
        uint64_t Low, High;
    };
}
