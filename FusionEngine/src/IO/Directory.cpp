#include "fepch.h"
#include "Directory.h"

namespace FusionEngine
{
    bool Directory::Create(const std::filesystem::path& path)
    {
        if (std::filesystem::exists(path)) {
            return false;
        }
        return std::filesystem::create_directory(path);
    }

    void Directory::EnsureCreated(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path)) {
            std::filesystem::create_directories(path);
        }
    }

    bool Directory::Delete(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path)) {
            return false;
        }
        return std::filesystem::remove_all(path) > 0;
    }

    bool Directory::Exists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }

    std::vector<std::filesystem::path> Directory::GetSubfolders(const std::filesystem::path& path)
    {
        std::vector<std::filesystem::path> subfolders;
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_directory(entry.path())) {
                subfolders.push_back(entry.path());
            }
        }
        return subfolders;
    }

    std::vector<std::filesystem::path> Directory::GetFiles(const std::filesystem::path& path)
    {
        std::vector<std::filesystem::path> files;
        if (std::filesystem::is_directory(path)) {
            for (auto& entry : std::filesystem::directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry.path())) {
                    files.push_back(entry.path());
                }
            }
        }
        return files;
    }

    std::vector<std::filesystem::path> Directory::GetFilesRecursive(const std::filesystem::path& path)
    {
        std::vector<std::filesystem::path> files;
        if (std::filesystem::is_directory(path)) {
            for (auto& entry : std::filesystem::recursive_directory_iterator(path)) {
                if (std::filesystem::is_regular_file(entry.path())) {
                    files.push_back(entry.path());
                }
            }
        }
        return files;
    }
}
