﻿#pragma once

// Undefine max macro from Windows.h ...
#undef max

#include <random>
#include <string>

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

        bool operator== (const UUID& other) const
        {
            return Low == other.Low && High == other.High;
        }

        bool operator!= (const UUID& other) const
        {
            return Low != other.Low || High != other.High;
        }

        std::string ToString() const { return std::to_string(Low) + std::to_string(High); }

        uint64_t Low, High;
    };
}
