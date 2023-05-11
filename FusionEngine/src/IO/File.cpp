#include "fepch.h"
#include "File.h"

namespace FusionEngine
{
    std::vector<char> File::Read(const std::filesystem::path& path)
    {
        if(!Exists(path))
            FE_ASSERT(false, "File not found!")
        
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        const std::size_t fileSize = static_cast<std::size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        
        return buffer;
    }

    void File::Write(const std::filesystem::path& path, std::vector<char>& data)
    {
        if(data.back() != '\0')
            data.push_back('\0');
        std::ofstream file(path);
        file << data.data();
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
