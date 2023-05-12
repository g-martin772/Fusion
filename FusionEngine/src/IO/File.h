﻿#pragma once

namespace FusionEngine
{
    class File
    {
    public:
        static std::vector<char> Read(const std::filesystem::path& path);
        static void Write(const std::filesystem::path& path, const std::vector<char>& data);
        static bool Exists(const std::filesystem::path& path);
        static void Create(const std::filesystem::path& path);
        static void EnsureCreated(const std::filesystem::path& path);
        static void Delete(const std::filesystem::path& path);
    };

}
