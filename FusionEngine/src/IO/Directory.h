#pragma once

#include <filesystem>

namespace FusionEngine
{
    class Directory {
    public:
        static bool Create(const std::filesystem::path& path);
        static void EnsureCreated(const std::filesystem::path& path);
        static bool Delete(const std::filesystem::path& path);
        static bool Exists(const std::filesystem::path& path);

        static std::vector<std::filesystem::path> GetSubfolders(const std::filesystem::path& path);
        static std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& path);
        static std::vector<std::filesystem::path> GetFilesRecursive(const std::filesystem::path& path);
    };
}