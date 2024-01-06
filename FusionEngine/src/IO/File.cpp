#include "fepch.h"
#include "File.h"

#include <filesystem>
#include <fstream>

namespace FusionEngine
{
    std::vector<char> File::Read(const std::filesystem::path& path)
    {
        FE_ASSERT(Exists(path), "File not found!");
        
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        const std::size_t fileSize = static_cast<std::size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), static_cast<int64_t>(fileSize));
        file.close();
        
        return buffer;
    }

    std::vector<std::string> File::ReadLines(const std::filesystem::path& path)
    {
        FE_ASSERT(Exists(path), "File not found!");

        std::ifstream file(path);
        std::vector<std::string> lines;
        std::string line;
    
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }

        file.close();

        return lines;
    }


    void File::Write(const std::filesystem::path& path, const std::vector<char>& data)
    {
        std::ofstream file(path, std::ios::binary);
        file.write(data.data(), static_cast<int64_t>(data.size()));
        file.close();
    }

    bool File::Exists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }

    void File::Create(const std::filesystem::path& path)
    {
        std::ofstream file(path);
        file.close();
    }

    void File::EnsureCreated(const std::filesystem::path& path)
    {
        if (!Exists(path)) {
            Create(path);
        }
    }

    void File::Delete(const std::filesystem::path& path)
    {
        if (Exists(path))
            std::filesystem::remove(path);
    }
}
