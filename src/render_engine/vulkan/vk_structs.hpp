/*!
 * \brief Vulkan definition of the structs forward-declared in render_engine.hpp
 */

#pragma once

#include "nova_renderer/rhi_types.hpp"

#include "vulkan.hpp"

namespace nova::renderer::rhi {
    struct VulkanDeviceMemory : DeviceMemory {
        VkDeviceMemory memory;
    };

    struct VulkanSampler : Sampler {
        VkSampler sampler;
    };

    struct VulkanImage : Image {
        VkImage image = VK_NULL_HANDLE;
        VkImageView image_view = VK_NULL_HANDLE;
        VulkanDeviceMemory* memory = nullptr;
    };

    struct VulkanBuffer : Buffer {
        VkBuffer buffer = VK_NULL_HANDLE;
        DeviceMemoryAllocation memory{};
    };

    struct VulkanRenderpass : Renderpass {
        VkRenderPass pass = VK_NULL_HANDLE;
        VkRect2D render_area{};
    };

    struct VulkanFramebuffer : Framebuffer {
        VkFramebuffer framebuffer = VK_NULL_HANDLE;
    };

    struct VulkanPipelineInterface : PipelineInterface {
        /*!
         * \brief Renderpass for the pipeline's output layouts because why _wouldn't_ that be married to the
         * renderpass itself?
         */
        VkRenderPass pass = VK_NULL_HANDLE;

        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;

        /*!
         * \brief All the descriptor set layouts that this pipeline interface needs to create descriptor sets
         *
         * The index in the vector is the index of the set
         */
        std::vector<VkDescriptorSetLayout> layouts_by_set;
    };

    struct VulkanPipeline : Pipeline {
        VkPipeline pipeline = VK_NULL_HANDLE;
    };

    struct VulkanDescriptorPool : DescriptorPool {
        VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    };

    struct VulkanDescriptorSet : DescriptorSet {
        VkDescriptorSet descriptor_set;
    };

    struct VulkanSemaphore : Semaphore {
        VkSemaphore semaphore;
    };

    struct VulkanFence : Fence {
        VkFence fence;
    };

    struct VulkanGpuInfo {
        VkPhysicalDevice phys_device{};
        std::vector<VkQueueFamilyProperties> queue_family_props;
        std::vector<VkExtensionProperties> available_extensions;
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> surface_formats;
        VkPhysicalDeviceProperties props{};
        VkPhysicalDeviceFeatures supported_features{};
        VkPhysicalDeviceMemoryProperties memory_properties{};
    };
} // namespace nova::renderer::rhi
