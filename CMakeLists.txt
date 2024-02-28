cmake_minimum_required(VERSION 3.28)
project(Fusion)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_SCAN_FOR_MODULES)
include(FetchContent)

FetchContent_Declare(
        vulkan-hpp
        GIT_REPOSITORY	https://github.com/KhronosGroup/Vulkan-Hpp
        GIT_TAG 	d4b36b82236e052a5e6e4cea5fe7967d5b565ebc
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/g-martin772/spdlog
        GIT_TAG d387fdf96c24d343ee6a988d4632b4969215bf5e
)

FetchContent_MakeAvailable(vulkan-hpp spdlog)

file(COPY
        "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Tools/MSVC/14.40.33521/modules/std.ixx"
        DESTINATION
        ${PROJECT_BINARY_DIR}/stdxx
)


set(vulkan_sdk "$ENV{VULKAN_SDK}")


# Fusion Engine

add_library(FusionEngine)

target_sources(FusionEngine PUBLIC
    FusionEngine/src/Core/Log.cpp
)

target_include_directories(FusionEngine PUBLIC
        ${vulkan-hpp_SOURCE_DIR}/glm
        ${vulkan-hpp_SOURCE_DIR}/Vulkan-Headers/include
        ${spdlog_SOURCE_DIR}/include
)

target_compile_definitions(FusionEngine PUBLIC
        GLM_GTC_INLINE_NAMESPACE
        GLM_EXT_INLINE_NAMESPACE
        GLM_GTX_INLINE_NAMESPACE

        VULKAN_HPP_NO_SMART_HANDLE
)

target_link_libraries(FusionEngine PUBLIC
        ${vulkan_sdk}/lib/vulkan-1.lib
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

# FusionForge

# Sandbox

add_executable(Sandbox Sandbox/src/Source.cpp)

target_sources(Sandbox PUBLIC FILE_SET CXX_MODULES FILES
        ${PROJECT_BINARY_DIR}/stdxx/std.ixx
        ${vulkan-hpp_SOURCE_DIR}/glm/glm/glm.cppm
)

target_include_directories(Sandbox PUBLIC
        FusionEngine/src/
        ${vulkan-hpp_SOURCE_DIR}/glm
        ${spdlog_SOURCE_DIR}/include
)

target_compile_definitions(Sandbox PUBLIC
        GLM_GTC_INLINE_NAMESPACE
        GLM_EXT_INLINE_NAMESPACE
        GLM_GTX_INLINE_NAMESPACE

)

target_link_libraries(Sandbox PUBLIC
        FusionEngine
)



# Dummy Project

add_executable(dummy Dummy/dummy.cpp)

target_sources(dummy PUBLIC FILE_SET CXX_MODULES FILES
        ${PROJECT_BINARY_DIR}/stdxx/std.ixx
        ${vulkan-hpp_SOURCE_DIR}/glm/glm/glm.cppm
        ${vulkan-hpp_SOURCE_DIR}/Vulkan-Headers/include/vulkan/vulkan.cppm
)

target_include_directories(dummy PUBLIC
        ${vulkan-hpp_SOURCE_DIR}/glm
        ${vulkan-hpp_SOURCE_DIR}/Vulkan-Headers/include
        ${spdlog_SOURCE_DIR}/include
)

target_compile_definitions(dummy PUBLIC
        GLM_GTC_INLINE_NAMESPACE
        GLM_EXT_INLINE_NAMESPACE
        GLM_GTX_INLINE_NAMESPACE

        VULKAN_HPP_NO_SMART_HANDLE
)

target_link_libraries(dummy PUBLIC
    ${vulkan_sdk}/lib/vulkan-1.lib
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)