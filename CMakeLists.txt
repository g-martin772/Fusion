cmake_minimum_required(VERSION 3.29-rc2)
project(Fusion)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_SCAN_FOR_MODULES)
include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	33b0eb9fa336ffd8551024b1d2690e418014553b
)

FetchContent_Declare(
        vulkan-hpp
        GIT_REPOSITORY	https://github.com/KhronosGroup/Vulkan-Hpp
        GIT_TAG 	d4b36b82236e052a5e6e4cea5fe7967d5b565ebc
)

FetchContent_MakeAvailable(glm vulkan-hpp)

file(COPY
        "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Tools/MSVC/14.40.33521/modules/std.ixx"
        DESTINATION
        ${PROJECT_BINARY_DIR}/stdxx
)


set(vulkan_sdk "$ENV{VULKAN_SDK}")


add_executable(dummy Dummy/dummy.cpp)

target_sources(dummy PUBLIC FILE_SET CXX_MODULES FILES
        ${PROJECT_BINARY_DIR}/stdxx/std.ixx
        ${glm_SOURCE_DIR}/glm/glm.cppm
        ${vulkan-hpp_SOURCE_DIR}/Vulkan-Headers/include/vulkan/vulkan.cppm
)

target_include_directories(dummy PUBLIC
        ${glm_SOURCE_DIR}/
        ${vulkan-hpp_SOURCE_DIR}/Vulkan-Headers/include
)

target_compile_definitions(dummy PUBLIC
        GLM_GTC_INLINE_NAMESPACE
        GLM_EXT_INLINE_NAMESPACE
        GLM_GTX_INLINE_NAMESPACE

        VULKAN_HPP_NO_SMART_HANDLE
)

target_link_libraries(dummy PUBLIC
    ${vulkan_sdk}/lib/vulkan-1.lib
)