#include "fepch.h"
#include "ImGui.h"


#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.cpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_vulkan.cpp>
#include <imgui/backends/imgui_impl_glfw.cpp>

#include "Core/Application.h"
#include "Platform/Vulkan/VulkanImage.h"
#include "Platform/Vulkan/VulkanRenderApi.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine { namespace UI {

    static vk::DescriptorPool descriptorPool;

    void CheckVkResultFn(VkResult result)
    {
        if (result != VK_SUCCESS)
        {
            FE_ASSERT(false, "Vulkan API call failed with error code: {0}", result);
        }
    }
    
    void ImGuiInitVulkan(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice, vk::RenderPass renderPass, uint32_t queueFamilyIndex, vk::Queue queue, uint32_t imageCount)
    {
        FE_INFO("Initializing ImGui for GLFW Vulkan");

        constexpr vk::DescriptorPoolSize poolSizes[]
        {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };
        
        vk::DescriptorPoolCreateInfo createInfo = {};
        createInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        createInfo.maxSets = 1000 * std::size(poolSizes);
        createInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        createInfo.pPoolSizes = poolSizes;
        descriptorPool = device.createDescriptorPool(createInfo);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  
        
        ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow()), true);
        
        ImGui_ImplVulkan_InitInfo initInfo;
        initInfo.Device = device;
        initInfo.PhysicalDevice = physicalDevice;
        initInfo.Instance = instance;
        initInfo.QueueFamily = queueFamilyIndex;
        initInfo.Queue = queue;
        initInfo.DescriptorPool = descriptorPool;
        initInfo.MinImageCount = imageCount;
        initInfo.ImageCount = imageCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = CheckVkResultFn;
        initInfo.PipelineCache = nullptr;
        initInfo.Subpass = 0;
        ImGui_ImplVulkan_Init(&initInfo, renderPass);
    }


    void ImGuiShutdownVulkan(vk::Device device)
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        device.destroyDescriptorPool(descriptorPool);
    }

    void ImGuiNewFrame()
    {
        //ImGui::UpdatePlatformWindows();
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderVulkan(vk::CommandBuffer commandBuffer)
    {
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void UploadImGuiFontsVulkan(vk::CommandPool commandPool, vk::CommandBuffer commandBuffer, vk::Device device, vk::Queue queue)
    {
        device.resetCommandPool(commandPool);
        constexpr vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        vk::Result result = commandBuffer.begin(&beginInfo);
        if (result != vk::Result::eSuccess)
        {
            FE_ERROR("Failed to begin command buffer for ImGui font upload");
            return;
        }

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        commandBuffer.end();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        result = queue.submit(1, &submitInfo, nullptr);
        if (result != vk::Result::eSuccess)
        {
            FE_ERROR("Failed to submit command buffer for ImGui font upload");
            return;
        }
        
        device.waitIdle();
        
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void* ImGuiGetImageHandle(const Ref<Image>& image)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: {
                const Ref<VulkanImage> vulkanImage = std::dynamic_pointer_cast<VulkanImage>(image);
                ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();
                ImGui_ImplVulkan_InitInfo* v = &bd->VulkanInitInfo;

                // Create Descriptor Set:
                VkDescriptorSet descriptor_set;
                {
                    VkDescriptorSetAllocateInfo alloc_info = {};
                    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                    alloc_info.descriptorPool = v->DescriptorPool;
                    alloc_info.descriptorSetCount = 1;
                    alloc_info.pSetLayouts = &bd->DescriptorSetLayout;
                    VkResult err = vkAllocateDescriptorSets(v->Device, &alloc_info, &descriptor_set);
                    check_vk_result(err);
                }

                // Update the Descriptor Set:
                {
                    VkDescriptorImageInfo desc_image = vulkanImage->GetDescriptorImageInfo();
                    VkWriteDescriptorSet write_desc[1] = {};
                    write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    write_desc[0].dstSet = descriptor_set;
                    write_desc[0].descriptorCount = 1;
                    write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    write_desc[0].pImageInfo = &desc_image;
                    vkUpdateDescriptorSets(v->Device, 1, write_desc, 0, nullptr);
                }
                return descriptor_set;
            }
        }
    }

    void ImGuiFreeImageHandle(void* handle)
    {
        switch (RenderApi::GetApi()) {
            case RenderApi::Api::Vulkan: {
                ImGui_ImplVulkan_Data* bd = ImGui_ImplVulkan_GetBackendData();
                ImGui_ImplVulkan_InitInfo* v = &bd->VulkanInitInfo;
                vkDeviceWaitIdle(v->Device);
                vkFreeDescriptorSets(v->Device, v->DescriptorPool, 1, reinterpret_cast<VkDescriptorSet*>(&handle));
                break;
            }
        }
    }
}}
