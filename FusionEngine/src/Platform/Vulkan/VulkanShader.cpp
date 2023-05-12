#include "fepch.h"
#include "VulkanShader.h"

#include "IO/File.h"

#include <shaderc/shaderc.hpp>

namespace FusionEngine
{
    static std::string cacheDirectory = "resources/shaders";
    static std::string sourceDirectory = "resources/shaders";

    std::vector<char> CompileShader(std::filesystem::path inputFile, std::filesystem::path outputFile, shaderc_shader_kind shaderType)
    {
        FE_INFO("Compiling shaderfile {0}", inputFile);
        const std::vector<char> glslSource = File::Read(inputFile);

        const shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

        const shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(glslSource.data(), glslSource.size(), shaderType, "shader", options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            FE_ERROR("Shader Compile Error: {0}", result.GetErrorMessage());
            FE_ASSERT(false, "Failed to compile GLSL to SPIR-V");
        }

        std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());

        std::vector<char> output;
        output.reserve(spirvCode.size() * sizeof(uint32_t));
        output.insert(output.end(), reinterpret_cast<const char*>(spirvCode.data()), reinterpret_cast<const char*>(spirvCode.data() + spirvCode.size()));

        
        File::Write(outputFile, output);
        return output;
    }
    
    VulkanShader::VulkanShader(const std::string& name): Shader(name)
    {
        const std::filesystem::path vertexSourceFilePath = fmt::format("{0}/{1}_vertex.glsl", sourceDirectory, name);
        const std::filesystem::path vertexSpirvFilePath = fmt::format("{0}/{1}_vertex.spv", cacheDirectory, name);

        const std::filesystem::path fragmentSourceFilePath = fmt::format("{0}/{1}_fragment.glsl", sourceDirectory, name);
        const std::filesystem::path fragmentSpirvFilePath = fmt::format("{0}/{1}_fragment.spv", cacheDirectory, name);

        if(File::Exists(vertexSpirvFilePath))
            m_VertexShader = File::Read(vertexSpirvFilePath);
        else
            m_VertexShader = CompileShader(vertexSourceFilePath, vertexSpirvFilePath, shaderc_vertex_shader);

        if(File::Exists(fragmentSpirvFilePath))
            m_FragmentShader = File::Read(fragmentSpirvFilePath);
        else
            m_FragmentShader = CompileShader(fragmentSourceFilePath, fragmentSpirvFilePath, shaderc_fragment_shader);
            
    }

    void VulkanShader::Load()
    {
    }

    void VulkanShader::Bind()
    {
    }
    
}
