/**
 * \internal
 * vd_vk.h - A helper vulkan library
 * 
 * zlib License
 * 
 * (C) Copyright 2025-2026 Michael Dodis (michaeldodisgr@gmail.com)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * @todo(mdodis): Introduce VkQueueConfiguration with multiple possible queue
 *                setups, which will allow the user to use dedicated or same
 *                queue for transfer compute etc.
 */
#ifndef VD_VK_H
#define VD_VK_H
#define VD_VK_VERSION_MAJOR    0
#define VD_VK_VERSION_MINOR    0
#define VD_VK_VERSION_PATCH    1
#define VD_VK_VERSION          ((VD_VK_VERSION_MAJOR << 16) | (VD_VK_VERSION_MINOR << 8) | (VD_VK_VERSION_PATCH))

#ifndef VD_H
#error "vd_vk.h requires vd.h"
#endif // !VD_H

#ifndef VULKAN_CORE_H_
#warning "vd_vulkan.h requires vulkan/vulkan.h"
#warning "vd_vulkan.h requires vulkan/vk_enum_string_helper.h"
#endif

#ifndef VD_VK_PROFILE_SCOPE_BEGIN
#define VD_VK_PROFILE_SCOPE_BEGIN(name)
#endif // !VD_VK_PROFILE_SCOPE_BEGIN

#ifndef VD_VK_PROFILE_SCOPE_END
#define VD_VK_PROFILE_SCOPE_END(name)
#endif // !VD_VK_PROFILE_SCOPE_END

#ifndef VD_VK_MACRO_ABBREVIATIONS
#define VD_VK_MACRO_ABBREVIATIONS VD_MACRO_ABBREVIATIONS
#endif // !VD_VK_MACRO_ABBREVIATIONS

#ifndef VD_VK_VMA_TRACKING
#define VD_VK_VMA_TRACKING 0
#endif // !VD_VK_VMA_TRACKING

#ifndef VD_VK_CHECK
#define VD_VK_CHECK(x)                                                                                                           \
    do {                                                                                                                         \
        VkResult vk_result = (x);                                                                                                       \
        if (vk_result != VK_SUCCESS) {                                                                                                  \
            printf("VK Assertion Failed: %s at %s:$%d\n(Function returned: %s)\n", #x, __FILE__, __LINE__, string_VkResult(vk_result)); \
            VD_DEBUG_BREAK();                                                                                                    \
        }                                                                                                                        \
    } while (0)
#endif // VD_VK_CHECK

#ifdef VD_VK_MACRO_ABBREVIATIONS
#define VK_CHECK(x) VD_VK_CHECK(x)
#endif // VD_VK_MACRO_ABBREVIATIONS

/* ----INITIALIZATION------------------------------------------------------------------------------------------------ */
typedef struct __VD_VK_GetPhysicalDevicePresentationSupportInfo {
    VkInstance          instance;
    VkPhysicalDevice    physical_device;
    Vdu32             queue_family_index;
} VdVkGetPhysicalDevicePresentationSupportInfo;

#define VD_VK_PROC_GET_PHYSICAL_DEVICE_PRESENTATION_SUPPORT(name) Vdb32 name(VdVkGetPhysicalDevicePresentationSupportInfo *info)
typedef VD_VK_PROC_GET_PHYSICAL_DEVICE_PRESENTATION_SUPPORT(VdVkProcGetPhysicalDevicePresentationSupport);

typedef struct __VD_VK_CreateWSISurfaceInfo {
    VkInstance instance;
} VdVkCreateWSISurfaceInfo;

#define VD_VK_PROC_CREATE_WSI_SURFACE(name) VkResult name(VdVkCreateWSISurfaceInfo *info, VkSurfaceKHR *surface)
typedef VD_VK_PROC_CREATE_WSI_SURFACE(VdVkProcCreateWSISurface);

typedef struct __VD_VK_RequireInstanceExtensionsInfo {
    Vdusize       count;
    const Vdcstr  *extensions;
    Vdb32         log;
} VdVkRequireInstanceExtensionsInfo;

typedef struct __VD_VK_RequireInstanceLayersInfo {
    Vdusize       count;
    const Vdcstr  *layers;
} VdVkRequireInstanceLayersInfo;

typedef struct __VD_VK_CreateInstanceExtendedInfo {
    const Vdcstr                              app_name;
    Vdu32                                     app_version;
    const Vdcstr                              engine_name;
    Vdu32                                     engine_version; 
    Vdu32                                     api_version;

    Vdusize                                   num_instance_extensions;
    const Vdcstr                              *instance_extensions;

    Vdusize                                   num_instance_layers;
    const Vdcstr                              *instance_layers;
    Vdb32                                     enable_validation;

    VkFlags                                   flags;

    /** Enables debug utils. For this to work "VK_EXT_debug_utils" must in @instance_extensions */
    struct {
        Vdb32                                 on;
        PFN_vkDebugUtilsMessengerCallbackEXT  messenger_callback;
    } debug_utils;

    struct {
        VkInstance                       *instance;
        VkDebugUtilsMessengerEXT         *debug_messenger;
        PFN_vkSetDebugUtilsObjectNameEXT *set_object_name;
        PFN_vkCmdBeginDebugUtilsLabelEXT *cmd_begin_label;
        PFN_vkCmdEndDebugUtilsLabelEXT   *cmd_end_label;
    } result;
} VdVkCreateInstanceExtendedInfo;

typedef enum {
    VD_VK_QUEUE_CAPABILITIES_GRAPHICS = 1 << 0,
    VD_VK_QUEUE_CAPABILITIES_COMPUTE  = 1 << 1,
    VD_VK_QUEUE_CAPABILITIES_TRANSFER = 1 << 2,
    VD_VK_QUEUE_CAPABILITIES_PRESENT  = 1 << 3,
} VdVkQueueCapabilities;

/** @doc VkQueueSetup
 * Describes a queue setup that is applied when:
 * - Checking if a physical device supports it
 * - Creating the logical device
 */
typedef struct __VD_VK_QueueSetup {
    /** The capability bits of the queue family required (or present) */
    VdVkQueueCapabilities     capabilities;
    /** The amount of queues required (or present) */
    Vdu32                     required_count;
    /** The amount of queues present */
    Vdu32                     present_count;
    /** The queue family index that matches these requirements */
    Vdi32                     cached_index;
} VdVkQueueSetup;

typedef struct __VD_VK_QueueGroup {
    Vdu32     queue_count;
    VkQueue   *queues;
} VdVkQueueGroup;

typedef struct __VD_VK_PhysicalDeviceCharacteristics {
    // VkPhysicalDeviceProperties
    Vdu32             api_version;

    // VkPhysicalDeviceFeatures
    Vdb32             robust_buffer_access;
    Vdb32             full_draw_index_uint32;
    Vdb32             image_cube_array;
    Vdb32             independent_blend;
    Vdb32             geometry_shader;
    Vdb32             tessellation_shader;
    Vdb32             sample_rate_shading;
    Vdb32             dual_src_blend;
    Vdb32             logic_op;
    Vdb32             multi_draw_indirect;
    Vdb32             draw_indirect_first_instance;
    Vdb32             depth_clamp;
    Vdb32             depth_bias_clamp;
    Vdb32             fill_mode_non_solid;
    Vdb32             depth_bounds;
    Vdb32             wide_lines;
    Vdb32             large_points;
    Vdb32             alpha_to_one;
    Vdb32             multi_viewport;
    Vdb32             sampler_anisotropy;
    Vdb32             texture_compression_etc2;
    Vdb32             texture_compression_astc_ldr;
    Vdb32             texture_compression_bc;
    Vdb32             occlusion_query_precise;
    Vdb32             pipeline_statistics_query;
    Vdb32             vertex_pipeline_stores_and_atomics;
    Vdb32             fragment_stores_and_atomics;
    Vdb32             shader_tessellation_and_geometry_point_size;
    Vdb32             shader_image_gather_extended;
    Vdb32             shader_storage_image_extended_formats;
    Vdb32             shader_storage_image_multisample;
    Vdb32             shader_storage_image_read_without_format;
    Vdb32             shader_storage_image_write_without_format;
    Vdb32             shader_uniform_buffer_array_dynamic_indexing;
    Vdb32             shader_sampled_image_array_dynamic_indexing;
    Vdb32             shader_storage_buffer_array_dynamic_indexing;
    Vdb32             shader_storage_image_array_dynamic_indexing;
    Vdb32             shader_clip_distance;
    Vdb32             shader_cull_distance;
    Vdb32             shader_float64;
    Vdb32             shader_int64;
    Vdb32             shader_int16;
    Vdb32             shader_resource_residency;
    Vdb32             shader_resource_min_lod;
    Vdb32             sparse_binding;
    Vdb32             sparse_residency_buffer;
    Vdb32             sparse_residency_image2_d;
    Vdb32             sparse_residency_image3_d;
    Vdb32             sparse_residency2_samples;
    Vdb32             sparse_residency4_samples;
    Vdb32             sparse_residency8_samples;
    Vdb32             sparse_residency16_samples;
    Vdb32             sparse_residency_aliased;
    Vdb32             variable_multisample_rate;
    Vdb32             inherited_queries;

    // VkPhysicalDeviceVulkan12Features
    Vdb32             sampler_mirror_clamp_to_edge;
    Vdb32             draw_indirect_count;
    Vdb32             storage_buffer8_bit_access;
    Vdb32             uniform_and_storage_buffer8_bit_access;
    Vdb32             storage_push_constant8;
    Vdb32             shader_buffer_int64_atomics;
    Vdb32             shader_shared_int64_atomics;
    Vdb32             shader_float16;
    Vdb32             shader_int8;
    Vdb32             descriptor_indexing;
    Vdb32             shader_input_attachment_array_dynamic_indexing;
    Vdb32             shader_uniform_texel_buffer_array_dynamic_indexing;
    Vdb32             shader_storage_texel_buffer_array_dynamic_indexing;
    Vdb32             shader_uniform_buffer_array_non_uniform_indexing;
    Vdb32             shader_sampled_image_array_non_uniform_indexing;
    Vdb32             shader_storage_buffer_array_non_uniform_indexing;
    Vdb32             shader_storage_image_array_non_uniform_indexing;
    Vdb32             shader_input_attachment_array_non_uniform_indexing;
    Vdb32             shader_uniform_texel_buffer_array_non_uniform_indexing;
    Vdb32             shader_storage_texel_buffer_array_non_uniform_indexing;
    Vdb32             descriptor_binding_uniform_buffer_update_after_bind;
    Vdb32             descriptor_binding_sampled_image_update_after_bind;
    Vdb32             descriptor_binding_storage_image_update_after_bind;
    Vdb32             descriptor_binding_storage_buffer_update_after_bind;
    Vdb32             descriptor_binding_uniform_texel_buffer_update_after_bind;
    Vdb32             descriptor_binding_storage_texel_buffer_update_after_bind;
    Vdb32             descriptor_binding_update_unused_while_pending;
    Vdb32             descriptor_binding_partially_bound;
    Vdb32             descriptor_binding_variable_descriptor_count;
    Vdb32             runtime_descriptor_array;
    Vdb32             sampler_filter_minmax;
    Vdb32             scalar_block_layout;
    Vdb32             imageless_framebuffer;
    Vdb32             uniform_buffer_standard_layout;
    Vdb32             shader_subgroup_extended_types;
    Vdb32             separate_depth_stencil_layouts;
    Vdb32             host_query_reset;
    Vdb32             timeline_semaphore;
    Vdb32             buffer_device_address;
    Vdb32             buffer_device_address_capture_replay;
    Vdb32             buffer_device_address_multi_device;
    Vdb32             vulkan_memory_model;
    Vdb32             vulkan_memory_model_device_scope;
    Vdb32             vulkan_memory_model_availability_visibility_chains;
    Vdb32             shader_output_viewport_index;
    Vdb32             shader_output_layer;
    Vdb32             subgroup_broadcast_dynamic_id;

    // VkPhysicalDeviceVulkan13Features
    Vdb32             robust_image_access;
    Vdb32             inline_uniform_block;
    Vdb32             descriptor_binding_inline_uniform_block_update_after_bind;
    Vdb32             pipeline_creation_cache_control;
    Vdb32             private_data;
    Vdb32             shader_demote_to_helper_invocation;
    Vdb32             shader_terminate_invocation;
    Vdb32             subgroup_size_control;
    Vdb32             compute_full_subgroups;
    Vdb32             synchronization2;
    Vdb32             texture_compression_astc_hdr;
    Vdb32             shader_zero_initialize_workgroup_memory;
    Vdb32             dynamic_rendering;
    Vdb32             shader_integer_dot_product;
    Vdb32             maintenance4;

    /** Multiplies rank by 2 to indicate preference */
    Vdb32             prefer_discrete;
    Vdb32             prefer_integrated;

    Vdusize           num_extensions;
    const Vdcstr      *extensions;

    Vdusize           num_queue_setups;
    VdVkQueueSetup    *queue_setups;

    /** The baseline preference for this compatible device adapter */
    Vdi32             rank_baseline;

    Vdusize           num_surface_formats;

    /** At least one of these formats must be supported. The first supported one will be written to @first_supported_surface_format */
    VkSurfaceFormatKHR  *surface_formats;

    /** Internal use only; Used to store rank of 
     * @VkPickPhysicalDeviceInfo.compatible_characteristics
     * computed_rank <= 0: physical device with characteristics is not available
     * computed_rank  > 0: physical device is available; higher rank is more preferential
     */
    Vdi32             computed_rank;
    VkPhysicalDevice    physical_device;

    Vdusize           first_supported_surface_format;
} VdVkPhysicalDeviceCharacteristics;

typedef struct __VD_VK_PickPhysicalDeviceInfo {
    VkInstance                                      instance;
    VkSurfaceKHR                                    surface;
    Vdusize                                       num_compatible_characteristics;
    VdVkPhysicalDeviceCharacteristics             *compatible_characteristics;
    VdVkProcGetPhysicalDevicePresentationSupport  *get_physical_device_presentation_support;
    Vdb32                                         log;
    struct {
        VkPhysicalDevice                          *physical_device;
        Vdusize                                   *selected_characteristics_index;
    } result;
} VdVkPickPhysicalDeviceInfo;

typedef struct __VD_VK_CreateDeviceAndQueuesInfo {
    VkInstance                          instance;
    VdVkPhysicalDeviceCharacteristics *characteristics;

    struct {
        VkDevice                        *device;
        Vdu32                         num_queue_groups;
        VdVkQueueGroup                *queue_groups;
    } result;
} VdVkCreateDeviceAndQueuesInfo;

typedef struct __VD_VK_CreateSwapchainAndFetchImagesInfo {
    VkDevice           device;
    VkPhysicalDevice   physical_device;
    VkSurfaceKHR       surface;
    VkSurfaceFormatKHR surface_format;
    VkExtent2D         extent;
    VkImageUsageFlags  image_usage;

    struct {
        VkSwapchainKHR  *swapchain;
        Vdu32         *num_images;
        VkImage         **images;
    } result;
} VdVkCreateSwapchainAndFetchImagesInfo;

typedef struct __VD_VK_CreateImageViewsInfo {
    VkDevice              device;
    Vdu32               num_images;
    VkImage               *images;
    VkFormat              image_format;

    VkImageViewCreateInfo *override_info;

    struct {
        VkImageView       **image_views;
    } result;

} VdVkCreateImageViewsInfo;

typedef struct __VD_VK_CmdImageTransitionInfo {
    VkImage            image;
    VkImageLayout      current_layout;
    VkImageLayout      target_layout;
    VkImageAspectFlags aspect;
    Vdu32            base_mip_level;
    Vdu32            level_count;
    Vdu32            base_array_layer;
    Vdu32            layer_count;
} VdVkCmdImageTransitionInfo;

Vdb32     vd_vk_require_instance_extensions         (VdArena *temp, VdVkRequireInstanceExtensionsInfo *info);
Vdb32     vd_vk_require_instance_layers             (VdArena *temp, VdVkRequireInstanceLayersInfo *info);
void      vd_vk_create_instance_extended            (VdVkCreateInstanceExtendedInfo *info);
void      vd_vk_log_physical_device_characteristics (VdVkPhysicalDeviceCharacteristics *characteristics);
Vdb32     vd_vk_pick_physical_device                (VdArena *temp, VdVkPickPhysicalDeviceInfo *info);
void      vd_vk_create_device_and_queues            (VdArena *temp, VdVkCreateDeviceAndQueuesInfo *info);
void      vd_vk_create_image_views                  (VdArena *a, VdVkCreateImageViewsInfo *info);
void      vd_vk_cmd_image_transition                (VkCommandBuffer cmd, VdVkCmdImageTransitionInfo *info);
void      vd_vk_destroy_image_views                 (VkDevice device, Vdu32 num_image_views, VkImageView *image_views);
void      vd_vk_destroy_framebuffers                (VkDevice device, Vdu32 num_framebuffers, VkFramebuffer *framebuffers);
void      vd_vk_destroy_semaphores                  (VkDevice device, Vdu32 num_semaphores, VkSemaphore *semaphores);

/**
 * @sym vk_create_swapchain_and_fetch_images
 * Create the swapchain and fetch
 * @param a The arena to allocate the images
 */
void      vd_vk_create_swapchain_and_fetch_images   (VdArena *a, VdVkCreateSwapchainAndFetchImagesInfo *info);

/* ----GROWABLE DESCRIPTOR ALLOCATOR--------------------------------------------------------------------------------- */
typedef struct __VDI_VK_GrowableDescriptorAllocatorPool {
    VkDescriptorPool pool;
    VdDListNode    node;
} VdVk__GrowableDescriptorAllocatorPool;

typedef struct __VD_VK_GrowableDescriptorAllocatorPoolSizeRatio {
    VkDescriptorType type;
    Vdf32          ratio;
} VdVkGrowableDescriptorAllocatorPoolSizeRatio;

typedef struct __VD_VK_GrowableDescriptorAllocator {
    VdArena                                      *arena;
    VkDevice                                       device;
    VdVk__GrowableDescriptorAllocatorPool         *current_pool;
    VdDList                                      used_pool_list;
    VdDList                                      free_pool_list;
    Vdusize                                      num_ratios;
    VdVkGrowableDescriptorAllocatorPoolSizeRatio *ratios;
} VdVkGrowableDescriptorAllocator;

typedef struct __VD_VK_GrowableDescriptorAllocatorInitInfo {
    VdArena                                      *arena;
    VkDevice                                       device;
    Vdusize                                      num_override_pool_sizes;
    VdVkGrowableDescriptorAllocatorPoolSizeRatio *override_pool_sizes;
} VdVkGrowableDescriptorAllocatorInitInfo;

void     vd_vk_growable_descriptor_allocator_init(VdVkGrowableDescriptorAllocator *desc_alloc, VdVkGrowableDescriptorAllocatorInitInfo *info);
VkResult vd_vk_growable_descriptor_allocator_get(VdVkGrowableDescriptorAllocator *desc_alloc, VkDescriptorSetLayout layout, VkDescriptorSet *set);
void     vd_vk_growable_descriptor_allocator_reset(VdVkGrowableDescriptorAllocator *desc_alloc);
void     vd_vk_growable_descriptor_allocator_deinit(VdVkGrowableDescriptorAllocator *desc_alloc);

/* ----DESCRIPTOR SET LAYOUT CACHE----------------------------------------------------------------------------------- */
#ifndef VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_CUSTOM
#define VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_CUSTOM 0
#endif // !VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_CUSTOM

#if !VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_CUSTOM
#define VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_MAX_BINDINGS 8
#endif // !VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_CUSTOM

typedef struct __VD_VK_DescriptorSetLayoutCacheKey {
    u32                          num_bindings;
    VkDescriptorSetLayoutBinding bindings[VD_VK_DESCRIPTOR_SET_LAYOUT_CACHE_MAX_BINDINGS];
} VdVk__DescriptorSetLayoutCacheKey;

typedef struct __VD_VK_DescriptorSetLayoutCacheKV {
    VdVk__DescriptorSetLayoutCacheKey k;
    VkDescriptorSetLayout              v;
} VdVk__DescriptorSetLayoutCacheKV;

typedef struct __VD_VK_DescriptorSetLayoutCache {
    VD_KVMAP VdVk__DescriptorSetLayoutCacheKV *map;
} VdVkDescriptorSetLayoutCache;

/* ----IMMEDIATE COMMANDS-------------------------------------------------------------------------------------------- */
typedef struct __VD_VK_ImmediateCommands {
    VkDevice        device;
    VkCommandPool   pool;
    VkCommandBuffer buffer;
    VkFence         fence;
    VkQueue         queue;
    Vdb32         recording;
} VdVkImmediateCommands;

typedef struct __VD_VK_ImmediateCommandsInitInfo {
    VkDevice device;
    Vdu32  queue_family_index;
    VkQueue  queue;
} VdVkImmediateCommandsInitInfo;

void            vd_vk_immediate_commands_init(VdVkImmediateCommands *cmds, VdVkImmediateCommandsInitInfo *info);
VkCommandBuffer vd_vk_immediate_commands_begin(VdVkImmediateCommands *cmds);
void            vd_vk_immediate_commands_end(VdVkImmediateCommands *cmds);
void            vd_vk_immediate_commands_deinit(VdVkImmediateCommands *cmds);

/* ----AMD VMA TRACKING---------------------------------------------------------------------------------------------- */
#if VD_VK_VMA_TRACKING
#ifndef AMD_VULKAN_MEMORY_ALLOCATOR_H
#error "VD_VK_VMA_TRACKING requires vk_mem_alloc.h"
#endif // !AMD_VULKAN_MEMORY_ALLOCATOR_H

#define VD_VK_VMA_TRACK_ALLOCATION(allocator, allocation)   vd_vk__vma_track_allocation(allocator, allocation, __FILE__, sizeof(__FILE__), __LINE__)
#define VD_VK_VMA_RELEASE_ALLOCATION(allocator, allocation) vd_vk__vma_release_allocation(allocator, allocation)
#define VD_VK_VMA_CHECK_ALLOCATIONS()                       vd_vk__vma_check_allocations()

void vd_vk__vma_track_allocation(VmaAllocator allocator, VmaAllocation allocation, const char *file, int filelen, int line);
void vd_vk__vma_release_allocation(VmaAllocator allocator, VmaAllocation allocation);
void vd_vk__vma_check_allocations(void);

#else

#define VD_VK_VMA_TRACK_ALLOCATION(allocator, allocation)   do {} while (0)
#define VD_VK_VMA_RELEASE_ALLOCATION(allocator, allocation) do {} while (0)
#define VD_VK_VMA_CHECK_ALLOCATIONS()                       do {} while (0)

#endif // VD_VK_VMA_TRACKING
/* ----AMD VMA TRACKING---------------------------------------------------------------------------------------------- */

#if VD_VK_MACRO_ABBREVIATIONS
#define VkGetPhysicalDevicePresentationSupportInfo  VdVkGetPhysicalDevicePresentationSupportInfo
#define VkProcGetPhysicalDevicePresentationSupport  VdVkProcGetPhysicalDevicePresentationSupport
#define VkCreateWSISurfaceInfo                      VdVkCreateWSISurfaceInfo
#define VkProcCreateWSISurface                      VdVkProcCreateWSISurface
#define VkRequireInstanceExtensionsInfo             VdVkRequireInstanceExtensionsInfo
#define VkRequireInstanceLayersInfo                 VdVkRequireInstanceLayersInfo
#define VkCreateInstanceExtendedInfo                VdVkCreateInstanceExtendedInfo
#define VkQueueCapabilities                         VdVkQueueCapabilities
#define VK_QUEUE_CAPABILITIES_GRAPHICS              VD_VK_QUEUE_CAPABILITIES_GRAPHICS
#define VK_QUEUE_CAPABILITIES_COMPUTE               VD_VK_QUEUE_CAPABILITIES_COMPUTE
#define VK_QUEUE_CAPABILITIES_TRANSFER              VD_VK_QUEUE_CAPABILITIES_TRANSFER
#define VK_QUEUE_CAPABILITIES_PRESENT               VD_VK_QUEUE_CAPABILITIES_PRESENT
#define VkQueueSetup                                VdVkQueueSetup
#define VkQueueGroup                                VdVkQueueGroup
#define VkPhysicalDeviceCharacteristics             VdVkPhysicalDeviceCharacteristics
#define VkPickPhysicalDeviceInfo                    VdVkPickPhysicalDeviceInfo
#define VkCreateDeviceAndQueuesInfo                 VdVkCreateDeviceAndQueuesInfo
#define VkCreateSwapchainAndFetchImagesInfo         VdVkCreateSwapchainAndFetchImagesInfo
#define VkCreateImageViewsInfo                      VdVkCreateImageViewsInfo
#define VkCmdImageTransitionInfo                    VdVkCmdImageTransitionInfo
#define vk_require_instance_extensions              vd_vk_require_instance_extensions
#define vk_require_instance_layers                  vd_vk_require_instance_layers
#define vk_create_instance_extended                 vd_vk_create_instance_extended
#define vk_log_physical_device_characteristics      vd_vk_log_physical_device_characteristics
#define vk_pick_physical_device                     vd_vk_pick_physical_device
#define vk_create_device_and_queues                 vd_vk_create_device_and_queues
#define vk_create_image_views                       vd_vk_create_image_views
#define vk_cmd_image_transition                     vd_vk_cmd_image_transition
#define vk_destroy_image_views                      vd_vk_destroy_image_views
#define vk_destroy_framebuffers                     vd_vk_destroy_framebuffers
#define vk_destroy_semaphores                       vd_vk_destroy_semaphores
#define vk_create_swapchain_and_fetch_images        vd_vk_create_swapchain_and_fetch_images
#define VkGrowableDescriptorAllocatorPoolSizeRatio  VdVkGrowableDescriptorAllocatorPoolSizeRatio
#define VkGrowableDescriptorAllocator               VdVkGrowableDescriptorAllocator
#define VkGrowableDescriptorAllocatorInitInfo       VdVkGrowableDescriptorAllocatorInitInfo
#define vk_growable_descriptor_allocator_init       vd_vk_growable_descriptor_allocator_init
#define vk_growable_descriptor_allocator_get        vd_vk_growable_descriptor_allocator_get
#define vk_growable_descriptor_allocator_reset      vd_vk_growable_descriptor_allocator_reset
#define vk_growable_descriptor_allocator_deinit     vd_vk_growable_descriptor_allocator_deinit
#define VkDescriptorSetLayoutCache                  VdVkDescriptorSetLayoutCache
#define VkImmediateCommands                         VdVkImmediateCommands
#define VkImmediateCommandsInitInfo                 VdVkImmediateCommandsInitInfo
#define vk_immediate_commands_init                  vd_vk_immediate_commands_init
#define vk_immediate_commands_begin                 vd_vk_immediate_commands_begin
#define vk_immediate_commands_end                   vd_vk_immediate_commands_end
#define vk_immediate_commands_deinit                vd_vk_immediate_commands_deinit
#endif // VD_VK_MACRO_ABBREVIATIONS

#endif // VD_VK_H

#ifdef VD_VK_IMPL

/* ----INITIALIZATION IMPL------------------------------------------------------------------------------------------- */
Vdb32 vd_vk_require_instance_extensions(VdArena *temp, VdVkRequireInstanceExtensionsInfo *info)
{
    VD_VK_PROFILE_SCOPE_BEGIN(Vd_Vk_Require_Instance_Extensions);
    VdArenaSave save = vd_arena_save(temp);

    Vdb32 result = VD_TRUE;

    Vdu32 num_instance_extensions;
    VkExtensionProperties *instance_extensions = 0;

    // Get all available extensions
    VD_VK_CHECK(vkEnumerateInstanceExtensionProperties(0, &num_instance_extensions, 0));
    instance_extensions = VD_ARENA_PUSH_ARRAY(temp, VkExtensionProperties, num_instance_extensions);
    VD_VK_CHECK(vkEnumerateInstanceExtensionProperties(0, &num_instance_extensions, instance_extensions));

    if (info->log) {
        for (Vdusize i = 0; i < info->count; ++i) {
            VD_DBGF("Required Extension       [%2zu] = %s", i, info->extensions[i]);
        }

        for (Vdusize i = 0; i < num_instance_extensions; ++i) {
            VD_DBGF("Vulkan Instance extension[%2zu] = %s", i, instance_extensions[i].extensionName);
        }
    }


    Vdb32 *ext_index_to_found = (Vdb32*)VD_ARENA_PUSH_ARRAY(temp, Vdb32, info->count);

    for (Vdusize i = 0; i < num_instance_extensions; ++i) {
        for (Vdusize j = 0; j < info->count; ++j) {
            if (ext_index_to_found[j]) {
                continue;
            }

            if (vd_cstr_cmp(info->extensions[j], instance_extensions[i].extensionName)) {
                ext_index_to_found[j] = VD_TRUE;
            }
        }
    }

    for (Vdusize i = 0; i < info->count; ++i) {
        if (!ext_index_to_found[i]) {
            result = VD_FALSE;
            break;
        }
    }

    vd_arena_restore(save);
    VD_VK_PROFILE_SCOPE_END(Vd_Vk_Require_Instance_Extensions);
    return result;
}


Vdb32 vd_vk_require_instance_layers(VdArena *temp, VdVkRequireInstanceLayersInfo *info)
{
    VD_VK_PROFILE_SCOPE_BEGIN(Vd_Vk_Require_Instance_layers);

    Vdb32 result = VD_TRUE;
    Vdu32 num_instance_layers;
    VkLayerProperties *instance_layers = 0;

    // Get all available extensions
    VD_VK_CHECK(vkEnumerateInstanceLayerProperties(&num_instance_layers, 0));
    instance_layers = VD_ARENA_PUSH_ARRAY(temp, VkLayerProperties, num_instance_layers);
    VD_VK_CHECK(vkEnumerateInstanceLayerProperties(&num_instance_layers, instance_layers));

    // Check each require layer against all available ones and make sure they exist
    for (Vdusize i = 0; i < info->count; ++i) {
        Vdb32 found = VD_FALSE;
        for (Vdusize j = 0; j < num_instance_layers; ++j) {
            if (vd_cstr_cmp(info->layers[i], instance_layers[j].layerName)) {
                found = VD_TRUE;
                break;
            }
        }

        if (!found) {
            result = VD_FALSE;
            break;
        }
    }

    VD_VK_PROFILE_SCOPE_END(Vd_Vk_Require_Instance_layers);
    return result;
}

void vd_vk_create_instance_extended(VdVkCreateInstanceExtendedInfo *info)
{
    VkInstance result;

    VkValidationFlagsEXT validation_flags = {
        .sType = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT,
        .disabledValidationCheckCount = 1,
        .pDisabledValidationChecks = (VkValidationCheckEXT[])
        {
            {
                VK_VALIDATION_CHECK_ALL_EXT,
            }
        },
    };

    VD_VK_PROFILE_SCOPE_BEGIN(Vd_Vk_Call_vkCreateInstance);
    VD_VK_CHECK(vkCreateInstance(& (VkInstanceCreateInfo){
        .sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                      = info->enable_validation ? (0) : (&validation_flags),
        .flags                      = info->flags,
        .pApplicationInfo = & (VkApplicationInfo) {
            .sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName       = info->app_name,
            .applicationVersion     = info->app_version,
            .pEngineName            = info->engine_name,
            .engineVersion          = info->engine_version,
            .apiVersion             = info->api_version,
        },
        .enabledLayerCount          = (Vdu32)info->num_instance_layers,
        .ppEnabledLayerNames        = (const char * const *)info->instance_layers,
        .enabledExtensionCount      = (Vdu32)info->num_instance_extensions,
        .ppEnabledExtensionNames    = (const char * const *)info->instance_extensions,
    }, 0, &result));
    *info->result.instance = result;
    VD_VK_PROFILE_SCOPE_END(Vd_Vk_Call_vkCreateInstance);

    VD_VK_PROFILE_SCOPE_BEGIN(Vd_Vk_Enable_DebugUtils);
    if (info->debug_utils.on) {
        PFN_vkCreateDebugUtilsMessengerEXT create_messenger;
        create_messenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(result, "vkCreateDebugUtilsMessengerEXT");
        VD_VK_CHECK(create_messenger(result, & (VkDebugUtilsMessengerCreateInfoEXT) {
            .sType              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity    =   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   | 
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
            .messageType        =   VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  | 
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback    = info->debug_utils.messenger_callback,
        }, 0, info->result.debug_messenger));

        if (info->result.set_object_name != 0) {
            PFN_vkSetDebugUtilsObjectNameEXT set_object_name;
            set_object_name = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(result, "vkSetDebugUtilsObjectNameEXT");
            *info->result.set_object_name = set_object_name;
        }

        if (info->result.cmd_begin_label != 0) {
            PFN_vkCmdBeginDebugUtilsLabelEXT cmd_begin_label = (PFN_vkCmdBeginDebugUtilsLabelEXT )vkGetInstanceProcAddr(result, "vkCmdBeginDebugUtilsLabelEXT");
            PFN_vkCmdEndDebugUtilsLabelEXT   cmd_end_label   = (PFN_vkCmdEndDebugUtilsLabelEXT   )vkGetInstanceProcAddr(result, "vkCmdEndDebugUtilsLabelEXT");
            *info->result.cmd_begin_label = cmd_begin_label;
            *info->result.cmd_end_label   = cmd_end_label;
        }
    }
    VD_VK_PROFILE_SCOPE_END(Vd_Vk_Enable_DebugUtils);
}

Vdb32 vd_vk_pick_physical_device(VdArena *temp, VdVkPickPhysicalDeviceInfo *info)
{
    // Get all physical devices
    Vdu32 num_phyiscal_devices;
    vkEnumeratePhysicalDevices(info->instance, &num_phyiscal_devices, 0);
    VkPhysicalDevice *physical_devices = VD_ARENA_PUSH_ARRAY(temp, VkPhysicalDevice, num_phyiscal_devices);
    vkEnumeratePhysicalDevices(info->instance, &num_phyiscal_devices, physical_devices);

    Vdi32             best_device_index   = -1;
    Vdi32             best_device_rank    = -1;
    VkPhysicalDevice    best_device_physical_device = VK_NULL_HANDLE;


    for (Vdu32 i = 0; i < num_phyiscal_devices; ++i) {
        VdArenaSave save = vd_arena_save(temp);

        // Get all physical device extensions
        Vdu32 num_device_extensions;
        vkEnumerateDeviceExtensionProperties(physical_devices[i], 0, &num_device_extensions, 0);
        VkExtensionProperties *device_extension_properties = VD_ARENA_PUSH_ARRAY(temp, VkExtensionProperties, num_device_extensions);
        vkEnumerateDeviceExtensionProperties(physical_devices[i], 0, &num_device_extensions, device_extension_properties);

        // Parse physical device extensions into plain cstr array
        Vdcstr *device_extensions = VD_ARENA_PUSH_ARRAY(temp, Vdcstr, num_device_extensions);
        for (Vdu32 j = 0; j < num_device_extensions; ++j) {
            device_extensions[j] = device_extension_properties[j].extensionName;

            // VUID-VkDeviceCreateInfo-pProperties-04451
            if (vd_cstr_cmp(device_extensions[j], "VK_KHR_portability_subset")) {

            }
        }

        // Get physical device properties
        VkPhysicalDeviceProperties2 device_properties = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        };
        vkGetPhysicalDeviceProperties2(physical_devices[i], &device_properties);

        // Get device features
        VkPhysicalDeviceVulkan12Features features12 = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        };

        VkPhysicalDeviceVulkan13Features features13 = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = &features12,
        };

        VkPhysicalDeviceFeatures2 features = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &features13,
        };
        vkGetPhysicalDeviceFeatures2(physical_devices[i], &features);

        // Get queue families
        u32 num_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &num_queue_families, 0);
        VkQueueFamilyProperties *queue_families = VD_ARENA_PUSH_ARRAY(temp, VkQueueFamilyProperties, num_queue_families);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &num_queue_families, queue_families);

        // Parse queue families into @VkQueueSetup array
        VdVkQueueSetup *queues = VD_ARENA_PUSH_ARRAY(temp, VdVkQueueSetup, num_queue_families);
        for (u32 j = 0; j < num_queue_families; ++j) {
            VdVkGetPhysicalDevicePresentationSupportInfo get_present_support_info = {
                .instance = info->instance,
                .physical_device = physical_devices[i],
                .queue_family_index = j,
            };

            VkQueueCapabilities caps = 0;

            if (queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) caps |= VD_VK_QUEUE_CAPABILITIES_GRAPHICS;
            if (queue_families[j].queueFlags & VK_QUEUE_TRANSFER_BIT) caps |= VD_VK_QUEUE_CAPABILITIES_TRANSFER;
            if (queue_families[j].queueFlags & VK_QUEUE_COMPUTE_BIT) caps |= VD_VK_QUEUE_CAPABILITIES_COMPUTE;
            if (info->get_physical_device_presentation_support(&get_present_support_info)) caps |= VD_VK_QUEUE_CAPABILITIES_PRESENT;

            queues[j] = (VdVkQueueSetup) {
                .capabilities = caps,
                .present_count = queue_families[j].queueCount,
                .cached_index = 0,
            };
        }

        Vdu32 num_surface_formats;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_devices[i], info->surface, &num_surface_formats, 0);
        VkSurfaceFormatKHR *surface_formats = VD_ARENA_PUSH_ARRAY(temp, VkSurfaceFormatKHR, num_surface_formats);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_devices[i], info->surface, &num_surface_formats, surface_formats);

        // Create @VkPhysicalDeviceCharacteristics
        VkPhysicalDeviceCharacteristics characteristics = {
            // VkPhysicalDeviceProperties
            .api_version                                  = device_properties.properties.apiVersion,

            // VkPhysicalDeviceFeatures
            .robust_buffer_access                         = features.features.robustBufferAccess,
            .full_draw_index_uint32                       = features.features.fullDrawIndexUint32,
            .image_cube_array                             = features.features.imageCubeArray,
            .independent_blend                            = features.features.independentBlend,
            .geometry_shader                              = features.features.geometryShader,
            .tessellation_shader                          = features.features.tessellationShader,
            .sample_rate_shading                          = features.features.sampleRateShading,
            .dual_src_blend                               = features.features.dualSrcBlend,
            .logic_op                                     = features.features.logicOp,
            .multi_draw_indirect                          = features.features.multiDrawIndirect,
            .draw_indirect_first_instance                 = features.features.drawIndirectFirstInstance,
            .depth_clamp                                  = features.features.depthClamp,
            .depth_bias_clamp                             = features.features.depthBiasClamp,
            .fill_mode_non_solid                          = features.features.fillModeNonSolid,
            .depth_bounds                                 = features.features.depthBounds,
            .wide_lines                                   = features.features.wideLines,
            .large_points                                 = features.features.largePoints,
            .alpha_to_one                                 = features.features.alphaToOne,
            .multi_viewport                               = features.features.multiViewport,
            .sampler_anisotropy                           = features.features.samplerAnisotropy,
            .texture_compression_etc2                     = features.features.textureCompressionETC2,
            .texture_compression_astc_ldr                 = features.features.textureCompressionASTC_LDR,
            .texture_compression_bc                       = features.features.textureCompressionBC,
            .occlusion_query_precise                      = features.features.occlusionQueryPrecise,
            .pipeline_statistics_query                    = features.features.pipelineStatisticsQuery,
            .vertex_pipeline_stores_and_atomics           = features.features.vertexPipelineStoresAndAtomics,
            .fragment_stores_and_atomics                  = features.features.fragmentStoresAndAtomics,
            .shader_tessellation_and_geometry_point_size  = features.features.shaderTessellationAndGeometryPointSize,
            .shader_image_gather_extended                 = features.features.shaderImageGatherExtended,
            .shader_storage_image_extended_formats        = features.features.shaderStorageImageExtendedFormats,
            .shader_storage_image_multisample             = features.features.shaderStorageImageMultisample,
            .shader_storage_image_read_without_format     = features.features.shaderStorageImageReadWithoutFormat,
            .shader_storage_image_write_without_format    = features.features.shaderStorageImageWriteWithoutFormat,
            .shader_uniform_buffer_array_dynamic_indexing = features.features.shaderUniformBufferArrayDynamicIndexing,
            .shader_sampled_image_array_dynamic_indexing  = features.features.shaderSampledImageArrayDynamicIndexing,
            .shader_storage_buffer_array_dynamic_indexing = features.features.shaderStorageBufferArrayDynamicIndexing,
            .shader_storage_image_array_dynamic_indexing  = features.features.shaderStorageImageArrayDynamicIndexing,
            .shader_clip_distance                         = features.features.shaderClipDistance,
            .shader_cull_distance                         = features.features.shaderCullDistance,
            .shader_float64                               = features.features.shaderFloat64,
            .shader_int64                                 = features.features.shaderInt64,
            .shader_int16                                 = features.features.shaderInt16,
            .shader_resource_residency                    = features.features.shaderResourceResidency,
            .shader_resource_min_lod                      = features.features.shaderResourceMinLod,
            .sparse_binding                               = features.features.sparseBinding,
            .sparse_residency_buffer                      = features.features.sparseResidencyBuffer,
            .sparse_residency_image2_d                    = features.features.sparseResidencyImage2D,
            .sparse_residency_image3_d                    = features.features.sparseResidencyImage3D,
            .sparse_residency2_samples                    = features.features.sparseResidency2Samples,
            .sparse_residency4_samples                    = features.features.sparseResidency4Samples,
            .sparse_residency8_samples                    = features.features.sparseResidency8Samples,
            .sparse_residency16_samples                   = features.features.sparseResidency16Samples,
            .sparse_residency_aliased                     = features.features.sparseResidencyAliased,
            .variable_multisample_rate                    = features.features.variableMultisampleRate,
            .inherited_queries                            = features.features.inheritedQueries,

            // VkPhysicalDeviceVulkan12Features
            .sampler_mirror_clamp_to_edge                              = features12.samplerMirrorClampToEdge,
            .draw_indirect_count                                       = features12.drawIndirectCount,
            .storage_buffer8_bit_access                                = features12.storageBuffer8BitAccess,
            .uniform_and_storage_buffer8_bit_access                    = features12.uniformAndStorageBuffer8BitAccess,
            .storage_push_constant8                                    = features12.storagePushConstant8,
            .shader_buffer_int64_atomics                               = features12.shaderBufferInt64Atomics,
            .shader_shared_int64_atomics                               = features12.shaderSharedInt64Atomics,
            .shader_float16                                            = features12.shaderFloat16,
            .shader_int8                                               = features12.shaderInt8,
            .descriptor_indexing                                       = features12.descriptorIndexing,
            .shader_input_attachment_array_dynamic_indexing            = features12.shaderInputAttachmentArrayDynamicIndexing,
            .shader_uniform_texel_buffer_array_dynamic_indexing        = features12.shaderUniformTexelBufferArrayDynamicIndexing,
            .shader_storage_texel_buffer_array_dynamic_indexing        = features12.shaderStorageTexelBufferArrayDynamicIndexing,
            .shader_uniform_buffer_array_non_uniform_indexing          = features12.shaderUniformBufferArrayNonUniformIndexing,
            .shader_sampled_image_array_non_uniform_indexing           = features12.shaderSampledImageArrayNonUniformIndexing,
            .shader_storage_buffer_array_non_uniform_indexing          = features12.shaderStorageBufferArrayNonUniformIndexing,
            .shader_storage_image_array_non_uniform_indexing           = features12.shaderStorageImageArrayNonUniformIndexing,
            .shader_input_attachment_array_non_uniform_indexing        = features12.shaderInputAttachmentArrayNonUniformIndexing,
            .shader_uniform_texel_buffer_array_non_uniform_indexing    = features12.shaderUniformTexelBufferArrayNonUniformIndexing,
            .shader_storage_texel_buffer_array_non_uniform_indexing    = features12.shaderStorageTexelBufferArrayNonUniformIndexing,
            .descriptor_binding_uniform_buffer_update_after_bind       = features12.descriptorBindingUniformBufferUpdateAfterBind,
            .descriptor_binding_sampled_image_update_after_bind        = features12.descriptorBindingSampledImageUpdateAfterBind,
            .descriptor_binding_storage_image_update_after_bind        = features12.descriptorBindingStorageImageUpdateAfterBind,
            .descriptor_binding_storage_buffer_update_after_bind       = features12.descriptorBindingStorageBufferUpdateAfterBind,
            .descriptor_binding_uniform_texel_buffer_update_after_bind = features12.descriptorBindingUniformTexelBufferUpdateAfterBind,
            .descriptor_binding_storage_texel_buffer_update_after_bind = features12.descriptorBindingStorageTexelBufferUpdateAfterBind,
            .descriptor_binding_update_unused_while_pending            = features12.descriptorBindingUpdateUnusedWhilePending,
            .descriptor_binding_partially_bound                        = features12.descriptorBindingPartiallyBound,
            .descriptor_binding_variable_descriptor_count              = features12.descriptorBindingVariableDescriptorCount,
            .runtime_descriptor_array                                  = features12.runtimeDescriptorArray,
            .sampler_filter_minmax                                     = features12.samplerFilterMinmax,
            .scalar_block_layout                                       = features12.scalarBlockLayout,
            .imageless_framebuffer                                     = features12.imagelessFramebuffer,
            .uniform_buffer_standard_layout                            = features12.uniformBufferStandardLayout,
            .shader_subgroup_extended_types                            = features12.shaderSubgroupExtendedTypes,
            .separate_depth_stencil_layouts                            = features12.separateDepthStencilLayouts,
            .host_query_reset                                          = features12.hostQueryReset,
            .timeline_semaphore                                        = features12.timelineSemaphore,
            .buffer_device_address                                     = features12.bufferDeviceAddress,
            .buffer_device_address_capture_replay                      = features12.bufferDeviceAddressCaptureReplay,
            .buffer_device_address_multi_device                        = features12.bufferDeviceAddressMultiDevice,
            .vulkan_memory_model                                       = features12.vulkanMemoryModel,
            .vulkan_memory_model_device_scope                          = features12.vulkanMemoryModelDeviceScope,
            .vulkan_memory_model_availability_visibility_chains        = features12.vulkanMemoryModelAvailabilityVisibilityChains,
            .shader_output_viewport_index                              = features12.shaderOutputViewportIndex,
            .shader_output_layer                                       = features12.shaderOutputLayer,
            .subgroup_broadcast_dynamic_id                             = features12.subgroupBroadcastDynamicId,

            // VkPhysicalDeviceVulkan13Features
            .robust_image_access                                       = features13.robustImageAccess,
            .inline_uniform_block                                      = features13.inlineUniformBlock,
            .descriptor_binding_inline_uniform_block_update_after_bind = features13.descriptorBindingInlineUniformBlockUpdateAfterBind,
            .pipeline_creation_cache_control                           = features13.pipelineCreationCacheControl,
            .private_data                                              = features13.privateData,
            .shader_demote_to_helper_invocation                        = features13.shaderDemoteToHelperInvocation,
            .shader_terminate_invocation                               = features13.shaderTerminateInvocation,
            .subgroup_size_control                                     = features13.subgroupSizeControl,
            .compute_full_subgroups                                    = features13.computeFullSubgroups,
            .synchronization2                                          = features13.synchronization2,
            .texture_compression_astc_hdr                              = features13.textureCompressionASTC_HDR,
            .shader_zero_initialize_workgroup_memory                   = features13.shaderZeroInitializeWorkgroupMemory,
            .dynamic_rendering                                         = features13.dynamicRendering,
            .shader_integer_dot_product                                = features13.shaderIntegerDotProduct,
            .maintenance4                                              = features13.maintenance4,
            .num_queue_setups       = num_queue_families,
            .queue_setups           = queues,
            .num_extensions         = num_device_extensions,
            .extensions             = device_extensions,

            .num_surface_formats    = num_surface_formats,
            .surface_formats        = surface_formats,

            .physical_device        = physical_devices[i],
        };

        if (info->log) {
            DBGF("vd_vk_pick_physical_device: Testing against candidate: %u: %s", i, device_properties.properties.deviceName);
            vd_vk_log_physical_device_characteristics(&characteristics);
        }

        // For each avaiable set, compare and rank it
        for (Vdusize j = 0; j < info->num_compatible_characteristics; ++j)
        {
            if (info->log) {
                DBGF("vd_vk_pick_physical_device: Checking compatible characteristics: %zu", j);
            }

            VkPhysicalDeviceCharacteristics *comparand = &info->compatible_characteristics[j];
            Vdi32 rank = comparand->rank_baseline;
            Vdb32 is_available = VD_TRUE;
            comparand->physical_device = physical_devices[i];

            // Check api version minimum
            if (comparand->api_version > characteristics.api_version) {
                is_available = false;
            }

            if (!is_available) {
                if (info->log) {
                    LOGF("vd_vk_pick_physical_device: Candidate device %u doesn't contain characteristic minimum api version: %zu ", i, j);
                }

                continue;
            }

            // Check capabilities
            #define CHECK_CAP(x) do { if (!(!comparand->x || characteristics.x)) is_available = VD_FALSE; } while (0)
            // VkPhysicalDeviceFeatures
            CHECK_CAP(robust_buffer_access);
            CHECK_CAP(full_draw_index_uint32);
            CHECK_CAP(image_cube_array);
            CHECK_CAP(independent_blend);
            CHECK_CAP(geometry_shader);
            CHECK_CAP(tessellation_shader);
            CHECK_CAP(sample_rate_shading);
            CHECK_CAP(dual_src_blend);
            CHECK_CAP(logic_op);
            CHECK_CAP(multi_draw_indirect);
            CHECK_CAP(draw_indirect_first_instance);
            CHECK_CAP(depth_clamp);
            CHECK_CAP(depth_bias_clamp);
            CHECK_CAP(fill_mode_non_solid);
            CHECK_CAP(depth_bounds);
            CHECK_CAP(wide_lines);
            CHECK_CAP(large_points);
            CHECK_CAP(alpha_to_one);
            CHECK_CAP(multi_viewport);
            CHECK_CAP(sampler_anisotropy);
            CHECK_CAP(texture_compression_etc2);
            CHECK_CAP(texture_compression_astc_ldr);
            CHECK_CAP(texture_compression_bc);
            CHECK_CAP(occlusion_query_precise);
            CHECK_CAP(pipeline_statistics_query);
            CHECK_CAP(vertex_pipeline_stores_and_atomics);
            CHECK_CAP(fragment_stores_and_atomics);
            CHECK_CAP(shader_tessellation_and_geometry_point_size);
            CHECK_CAP(shader_image_gather_extended);
            CHECK_CAP(shader_storage_image_extended_formats);
            CHECK_CAP(shader_storage_image_multisample);
            CHECK_CAP(shader_storage_image_read_without_format);
            CHECK_CAP(shader_storage_image_write_without_format);
            CHECK_CAP(shader_uniform_buffer_array_dynamic_indexing);
            CHECK_CAP(shader_sampled_image_array_dynamic_indexing);
            CHECK_CAP(shader_storage_buffer_array_dynamic_indexing);
            CHECK_CAP(shader_storage_image_array_dynamic_indexing);
            CHECK_CAP(shader_clip_distance);
            CHECK_CAP(shader_cull_distance);
            CHECK_CAP(shader_float64);
            CHECK_CAP(shader_int64);
            CHECK_CAP(shader_int16);
            CHECK_CAP(shader_resource_residency);
            CHECK_CAP(shader_resource_min_lod);
            CHECK_CAP(sparse_binding);
            CHECK_CAP(sparse_residency_buffer);
            CHECK_CAP(sparse_residency_image2_d);
            CHECK_CAP(sparse_residency_image3_d);
            CHECK_CAP(sparse_residency2_samples);
            CHECK_CAP(sparse_residency4_samples);
            CHECK_CAP(sparse_residency8_samples);
            CHECK_CAP(sparse_residency16_samples);
            CHECK_CAP(sparse_residency_aliased);
            CHECK_CAP(variable_multisample_rate);
            CHECK_CAP(inherited_queries);

            // VkPhysicalDeviceVulkan12Features
            CHECK_CAP(sampler_mirror_clamp_to_edge);
            CHECK_CAP(draw_indirect_count);
            CHECK_CAP(storage_buffer8_bit_access);
            CHECK_CAP(uniform_and_storage_buffer8_bit_access);
            CHECK_CAP(storage_push_constant8);
            CHECK_CAP(shader_buffer_int64_atomics);
            CHECK_CAP(shader_shared_int64_atomics);
            CHECK_CAP(shader_float16);
            CHECK_CAP(shader_int8);
            CHECK_CAP(descriptor_indexing);
            CHECK_CAP(shader_input_attachment_array_dynamic_indexing);
            CHECK_CAP(shader_uniform_texel_buffer_array_dynamic_indexing);
            CHECK_CAP(shader_storage_texel_buffer_array_dynamic_indexing);
            CHECK_CAP(shader_uniform_buffer_array_non_uniform_indexing);
            CHECK_CAP(shader_sampled_image_array_non_uniform_indexing);
            CHECK_CAP(shader_storage_buffer_array_non_uniform_indexing);
            CHECK_CAP(shader_storage_image_array_non_uniform_indexing);
            CHECK_CAP(shader_input_attachment_array_non_uniform_indexing);
            CHECK_CAP(shader_uniform_texel_buffer_array_non_uniform_indexing);
            CHECK_CAP(shader_storage_texel_buffer_array_non_uniform_indexing);
            CHECK_CAP(descriptor_binding_uniform_buffer_update_after_bind);
            CHECK_CAP(descriptor_binding_sampled_image_update_after_bind);
            CHECK_CAP(descriptor_binding_storage_image_update_after_bind);
            CHECK_CAP(descriptor_binding_storage_buffer_update_after_bind);
            CHECK_CAP(descriptor_binding_uniform_texel_buffer_update_after_bind);
            CHECK_CAP(descriptor_binding_storage_texel_buffer_update_after_bind);
            CHECK_CAP(descriptor_binding_update_unused_while_pending);
            CHECK_CAP(descriptor_binding_partially_bound);
            CHECK_CAP(descriptor_binding_variable_descriptor_count);
            CHECK_CAP(runtime_descriptor_array);
            CHECK_CAP(sampler_filter_minmax);
            CHECK_CAP(scalar_block_layout);
            CHECK_CAP(imageless_framebuffer);
            CHECK_CAP(uniform_buffer_standard_layout);
            CHECK_CAP(shader_subgroup_extended_types);
            CHECK_CAP(separate_depth_stencil_layouts);
            CHECK_CAP(host_query_reset);
            CHECK_CAP(timeline_semaphore);
            CHECK_CAP(buffer_device_address);
            CHECK_CAP(buffer_device_address_capture_replay);
            CHECK_CAP(buffer_device_address_multi_device);
            CHECK_CAP(vulkan_memory_model);
            CHECK_CAP(vulkan_memory_model_device_scope);
            CHECK_CAP(vulkan_memory_model_availability_visibility_chains);
            CHECK_CAP(shader_output_viewport_index);
            CHECK_CAP(shader_output_layer);
            CHECK_CAP(subgroup_broadcast_dynamic_id);

            // VkPhysicalDeviceVulkan13Features
            CHECK_CAP(robust_image_access);
            CHECK_CAP(inline_uniform_block);
            CHECK_CAP(descriptor_binding_inline_uniform_block_update_after_bind);
            CHECK_CAP(pipeline_creation_cache_control);
            CHECK_CAP(private_data);
            CHECK_CAP(shader_demote_to_helper_invocation);
            CHECK_CAP(shader_terminate_invocation);
            CHECK_CAP(subgroup_size_control);
            CHECK_CAP(compute_full_subgroups);
            CHECK_CAP(synchronization2);
            CHECK_CAP(texture_compression_astc_hdr);
            CHECK_CAP(shader_zero_initialize_workgroup_memory);
            CHECK_CAP(dynamic_rendering);
            CHECK_CAP(shader_integer_dot_product);
            CHECK_CAP(maintenance4);

            CHECK_CAP(synchronization2);
            CHECK_CAP(dynamic_rendering);
            #undef CHECK_CAP

            if (!is_available) {
                if (info->log) {
                    LOGF("vd_vk_pick_physical_device: Candidate device %u doesn't contain characteristic capabilities: %zu ", i, j);
                }

                continue;
            }

            // Check extensions
            for (Vdusize k = 0; k < comparand->num_extensions; ++k) {
                Vdb32 found = VD_FALSE;
                for (Vdusize z = 0; z < num_device_extensions; ++z) {
                    if (vd_cstr_cmp(device_extension_properties[z].extensionName, comparand->extensions[k])) {
                        found = VD_TRUE;
                        break;
                    }
                }

                if (!found) {
                    is_available = VD_FALSE;
                    break;
                }
            }

            if (!is_available) {
                if (info->log) {
                    LOGF("vd_vk_pick_physical_device: Candidate device %u doesn't contain characteristic extensions: %zu ", i, j);
                }

                continue;
            }

            // Check surface formats
            if (info->surface != VK_NULL_HANDLE) {
                Vdusize first_compatible_surface_format_index = comparand->num_surface_formats;

                for (Vdusize k = 0; k < comparand->num_surface_formats; ++k) {
                    Vdb32 found = VD_FALSE;
                    for (Vdusize z = 0; z < characteristics.num_surface_formats; ++z) {
                        if (comparand->surface_formats[k].format == characteristics.surface_formats[z].format &&
                            comparand->surface_formats[k].colorSpace == characteristics.surface_formats[z].colorSpace)
                        {
                            found = VD_TRUE;
                            break;
                        }
                    }

                    if (found) {
                        first_compatible_surface_format_index = k;
                        break;
                    }
                }

                if (first_compatible_surface_format_index == comparand->num_surface_formats) {
                    if (info->log) {
                        LOGF("vd_vk_pick_physical_device: Candidate device %u doesn't contain characteristic[%zu] compatible surface formats", i, j);
                        continue;
                    }
                }

                comparand->first_supported_surface_format = first_compatible_surface_format_index;
            }

            // Check queues
            for (Vdusize k = 0; k < comparand->num_queue_setups; ++k) {
                VdVkQueueSetup *want_queue_family = &comparand->queue_setups[k];
                Vdi32 queue_index = -1;

                // Loop over all available queues
                for (Vdusize z = 0; z < characteristics.num_queue_setups; ++z) {

                    // If this queue was already picked, don't bother checking
                    if (characteristics.queue_setups[z].cached_index > 0) {
                        if (info->log) {
                            LOGF("vd_vk_pick_physical_device: Search queue family[%zu] family with index %zu is already used", k, z);
                        }
                        continue;
                    }

                    // If required_count > present_count then abort
                    if (want_queue_family->required_count > characteristics.queue_setups[z].present_count) {
                        if (info->log) {
                            LOGF("vd_vk_pick_physical_device: Search queue family[%zu] family with index %zu doesn't contain enough queues", k, z);
                        }
                        continue;
                    }

                    // Any queue must match the required capabilities
                    if (characteristics.queue_setups[z].capabilities & want_queue_family->capabilities) {
                        queue_index = (Vdi32)z;
                        characteristics.queue_setups[z].cached_index = 1;
                        if (info->log) {
                            LOGF("vd_vk_pick_physical_device: Search queue family[%zu] matched family with index %zu", k, z);
                        }
                        break;
                    }
                }

                // If no queue family was found, then abort
                if (queue_index == -1) {

                    if (info->log) {
                        LOGF("vd_vk_pick_physical_device: Queue family with index %zu could not be found", k);
                    }

                    is_available = false;
                    break;
                }

                // Store queue family index for the user to create it
                want_queue_family->cached_index = queue_index;
                want_queue_family->present_count = characteristics.queue_setups[queue_index].present_count;
            }

            if (!is_available) {
                if (info->log) {
                    LOGF("vd_vk_pick_physical_device: Candidate device %u doesn't have any matching characteristic queue setup: %zu ", i, j);
                }

                continue;
            }

            if (comparand->prefer_discrete) {
                rank += 10 * (device_properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1 : 0);
            }

            if (comparand->prefer_integrated) {
                rank += 10 * (device_properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? 1 : 0);
            }

            if (info->log) {
                LOGF("Compatible Device[%zu] Rank = %d", j, rank);
            }

            if (rank > best_device_rank) {
                best_device_index = (Vdi32)j;
                best_device_physical_device = physical_devices[i];
                best_device_rank = rank;
            }
        }

        vd_arena_restore(save);
    }

    if (best_device_index < 0) {
        return VD_FALSE;
    }

    LOGF("vd_vk_pick_physical_device: Selected device: %d", (Vdi32)best_device_index);

    *info->result.physical_device = best_device_physical_device;
    *info->result.selected_characteristics_index = (Vdi32)best_device_index;

    return VD_TRUE;
}

void vd_vk_log_physical_device_characteristics(VdVkPhysicalDeviceCharacteristics *characteristics)
{
    LOGF("%-60s", "Capabilities---------");
    LOGF("%-60s", "VkPhysicalDeviceFeatures {");
    LOGF("%-60s= %d,", "\trobustBufferAccess",                      characteristics->robust_buffer_access);
    LOGF("%-60s= %d,", "\tfullDrawIndexUint32",                     characteristics->full_draw_index_uint32);
    LOGF("%-60s= %d,", "\timageCubeArray",                          characteristics->image_cube_array);
    LOGF("%-60s= %d,", "\tindependentBlend",                        characteristics->independent_blend);
    LOGF("%-60s= %d,", "\tgeometryShader",                          characteristics->geometry_shader);
    LOGF("%-60s= %d,", "\ttessellationShader",                      characteristics->tessellation_shader);
    LOGF("%-60s= %d,", "\tsampleRateShading",                       characteristics->sample_rate_shading);
    LOGF("%-60s= %d,", "\tdualSrcBlend",                            characteristics->dual_src_blend);
    LOGF("%-60s= %d,", "\tlogicOp",                                 characteristics->logic_op);
    LOGF("%-60s= %d,", "\tmultiDrawIndirect",                       characteristics->multi_draw_indirect);
    LOGF("%-60s= %d,", "\tdrawIndirectFirstInstance",               characteristics->draw_indirect_first_instance);
    LOGF("%-60s= %d,", "\tdepthClamp",                              characteristics->depth_clamp);
    LOGF("%-60s= %d,", "\tdepthBiasClamp",                          characteristics->depth_bias_clamp);
    LOGF("%-60s= %d,", "\tfillModeNonSolid",                        characteristics->fill_mode_non_solid);
    LOGF("%-60s= %d,", "\tdepthBounds",                             characteristics->depth_bounds);
    LOGF("%-60s= %d,", "\twideLines",                               characteristics->wide_lines);
    LOGF("%-60s= %d,", "\tlargePoints",                             characteristics->large_points);
    LOGF("%-60s= %d,", "\talphaToOne",                              characteristics->alpha_to_one);
    LOGF("%-60s= %d,", "\tmultiViewport",                           characteristics->multi_viewport);
    LOGF("%-60s= %d,", "\tsamplerAnisotropy",                       characteristics->sampler_anisotropy);
    LOGF("%-60s= %d,", "\ttextureCompressionETC2",                  characteristics->texture_compression_etc2);
    LOGF("%-60s= %d,", "\ttextureCompressionASTC_LDR",              characteristics->texture_compression_astc_ldr);
    LOGF("%-60s= %d,", "\ttextureCompressionBC",                    characteristics->texture_compression_bc);
    LOGF("%-60s= %d,", "\tocclusionQueryPrecise",                   characteristics->occlusion_query_precise);
    LOGF("%-60s= %d,", "\tpipelineStatisticsQuery",                 characteristics->pipeline_statistics_query);
    LOGF("%-60s= %d,", "\tvertexPipelineStoresAndAtomics",          characteristics->vertex_pipeline_stores_and_atomics);
    LOGF("%-60s= %d,", "\tfragmentStoresAndAtomics",                characteristics->fragment_stores_and_atomics);
    LOGF("%-60s= %d,", "\tshaderTessellationAndGeometryPointSize",  characteristics->shader_tessellation_and_geometry_point_size);
    LOGF("%-60s= %d,", "\tshaderImageGatherExtended",               characteristics->shader_image_gather_extended);
    LOGF("%-60s= %d,", "\tshaderStorageImageExtendedFormats",       characteristics->shader_storage_image_extended_formats);
    LOGF("%-60s= %d,", "\tshaderStorageImageMultisample",           characteristics->shader_storage_image_multisample);
    LOGF("%-60s= %d,", "\tshaderStorageImageReadWithoutFormat",     characteristics->shader_storage_image_read_without_format);
    LOGF("%-60s= %d,", "\tshaderStorageImageWriteWithoutFormat",    characteristics->shader_storage_image_write_without_format);
    LOGF("%-60s= %d,", "\tshaderUniformBufferArrayDynamicIndexing", characteristics->shader_uniform_buffer_array_dynamic_indexing);
    LOGF("%-60s= %d,", "\tshaderSampledImageArrayDynamicIndexing",  characteristics->shader_sampled_image_array_dynamic_indexing);
    LOGF("%-60s= %d,", "\tshaderStorageBufferArrayDynamicIndexing", characteristics->shader_storage_buffer_array_dynamic_indexing);
    LOGF("%-60s= %d,", "\tshaderStorageImageArrayDynamicIndexing",  characteristics->shader_storage_image_array_dynamic_indexing);
    LOGF("%-60s= %d,", "\tshaderClipDistance",                      characteristics->shader_clip_distance);
    LOGF("%-60s= %d,", "\tshaderCullDistance",                      characteristics->shader_cull_distance);
    LOGF("%-60s= %d,", "\tshaderFloat64",                           characteristics->shader_float64);
    LOGF("%-60s= %d,", "\tshaderInt64",                             characteristics->shader_int64);
    LOGF("%-60s= %d,", "\tshaderInt16",                             characteristics->shader_int16);
    LOGF("%-60s= %d,", "\tshaderResourceResidency",                 characteristics->shader_resource_residency);
    LOGF("%-60s= %d,", "\tshaderResourceMinLod",                    characteristics->shader_resource_min_lod);
    LOGF("%-60s= %d,", "\tsparseBinding",                           characteristics->sparse_binding);
    LOGF("%-60s= %d,", "\tsparseResidencyBuffer",                   characteristics->sparse_residency_buffer);
    LOGF("%-60s= %d,", "\tsparseResidencyImage2D",                  characteristics->sparse_residency_image2_d);
    LOGF("%-60s= %d,", "\tsparseResidencyImage3D",                  characteristics->sparse_residency_image3_d);
    LOGF("%-60s= %d,", "\tsparseResidency2Samples",                 characteristics->sparse_residency2_samples);
    LOGF("%-60s= %d,", "\tsparseResidency4Samples",                 characteristics->sparse_residency4_samples);
    LOGF("%-60s= %d,", "\tsparseResidency8Samples",                 characteristics->sparse_residency8_samples);
    LOGF("%-60s= %d,", "\tsparseResidency16Samples",                characteristics->sparse_residency16_samples);
    LOGF("%-60s= %d,", "\tsparseResidencyAliased",                  characteristics->sparse_residency_aliased);
    LOGF("%-60s= %d,", "\tvariableMultisampleRate",                 characteristics->variable_multisample_rate);
    LOGF("%-60s= %d,", "\tinheritedQueries",                        characteristics->inherited_queries);
    LOGF("%-60s", "} // VkPhysicalDeviceFeatures");

    LOGF("%-60s", "VkPhysicalDeviceVulkan12Features {");
    LOGF("%-60s= %d", "\tsamplerMirrorClampToEdge",                           characteristics->sampler_mirror_clamp_to_edge);
    LOGF("%-60s= %d", "\tdrawIndirectCount",                                  characteristics->draw_indirect_count);
    LOGF("%-60s= %d", "\tstorageBuffer8BitAccess",                            characteristics->storage_buffer8_bit_access);
    LOGF("%-60s= %d", "\tuniformAndStorageBuffer8BitAccess",                  characteristics->uniform_and_storage_buffer8_bit_access);
    LOGF("%-60s= %d", "\tstoragePushConstant8",                               characteristics->storage_push_constant8);
    LOGF("%-60s= %d", "\tshaderBufferInt64Atomics",                           characteristics->shader_buffer_int64_atomics);
    LOGF("%-60s= %d", "\tshaderSharedInt64Atomics",                           characteristics->shader_shared_int64_atomics);
    LOGF("%-60s= %d", "\tshaderFloat16",                                      characteristics->shader_float16);
    LOGF("%-60s= %d", "\tshaderInt8",                                         characteristics->shader_int8);
    LOGF("%-60s= %d", "\tdescriptorIndexing",                                 characteristics->descriptor_indexing);
    LOGF("%-60s= %d", "\tshaderInputAttachmentArrayDynamicIndexing",          characteristics->shader_input_attachment_array_dynamic_indexing);
    LOGF("%-60s= %d", "\tshaderUniformTexelBufferArrayDynamicIndexing",       characteristics->shader_uniform_texel_buffer_array_dynamic_indexing);
    LOGF("%-60s= %d", "\tshaderStorageTexelBufferArrayDynamicIndexing",       characteristics->shader_storage_texel_buffer_array_dynamic_indexing);
    LOGF("%-60s= %d", "\tshaderUniformBufferArrayNonUniformIndexing",         characteristics->shader_uniform_buffer_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tshaderSampledImageArrayNonUniformIndexing",          characteristics->shader_sampled_image_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tshaderStorageBufferArrayNonUniformIndexing",         characteristics->shader_storage_buffer_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tshaderStorageImageArrayNonUniformIndexing",          characteristics->shader_storage_image_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tshaderInputAttachmentArrayNonUniformIndexing",       characteristics->shader_input_attachment_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tshaderUniformTexelBufferArrayNonUniformIndexing",    characteristics->shader_uniform_texel_buffer_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tshaderStorageTexelBufferArrayNonUniformIndexing",    characteristics->shader_storage_texel_buffer_array_non_uniform_indexing);
    LOGF("%-60s= %d", "\tdescriptorBindingUniformBufferUpdateAfterBind",      characteristics->descriptor_binding_uniform_buffer_update_after_bind);
    LOGF("%-60s= %d", "\tdescriptorBindingSampledImageUpdateAfterBind",       characteristics->descriptor_binding_sampled_image_update_after_bind);
    LOGF("%-60s= %d", "\tdescriptorBindingStorageImageUpdateAfterBind",       characteristics->descriptor_binding_storage_image_update_after_bind);
    LOGF("%-60s= %d", "\tdescriptorBindingStorageBufferUpdateAfterBind",      characteristics->descriptor_binding_storage_buffer_update_after_bind);
    LOGF("%-60s= %d", "\tdescriptorBindingUniformTexelBufferUpdateAfterBind", characteristics->descriptor_binding_uniform_texel_buffer_update_after_bind);
    LOGF("%-60s= %d", "\tdescriptorBindingStorageTexelBufferUpdateAfterBind", characteristics->descriptor_binding_storage_texel_buffer_update_after_bind);
    LOGF("%-60s= %d", "\tdescriptorBindingUpdateUnusedWhilePending",          characteristics->descriptor_binding_update_unused_while_pending);
    LOGF("%-60s= %d", "\tdescriptorBindingPartiallyBound",                    characteristics->descriptor_binding_partially_bound);
    LOGF("%-60s= %d", "\tdescriptorBindingVariableDescriptorCount",           characteristics->descriptor_binding_variable_descriptor_count);
    LOGF("%-60s= %d", "\truntimeDescriptorArray",                             characteristics->runtime_descriptor_array);
    LOGF("%-60s= %d", "\tsamplerFilterMinmax",                                characteristics->sampler_filter_minmax);
    LOGF("%-60s= %d", "\tscalarBlockLayout",                                  characteristics->scalar_block_layout);
    LOGF("%-60s= %d", "\timagelessFramebuffer",                               characteristics->imageless_framebuffer);
    LOGF("%-60s= %d", "\tuniformBufferStandardLayout",                        characteristics->uniform_buffer_standard_layout);
    LOGF("%-60s= %d", "\tshaderSubgroupExtendedTypes",                        characteristics->shader_subgroup_extended_types);
    LOGF("%-60s= %d", "\tseparateDepthStencilLayouts",                        characteristics->separate_depth_stencil_layouts);
    LOGF("%-60s= %d", "\thostQueryReset",                                     characteristics->host_query_reset);
    LOGF("%-60s= %d", "\ttimelineSemaphore",                                  characteristics->timeline_semaphore);
    LOGF("%-60s= %d", "\tbufferDeviceAddress",                                characteristics->buffer_device_address);
    LOGF("%-60s= %d", "\tbufferDeviceAddressCaptureReplay",                   characteristics->buffer_device_address_capture_replay);
    LOGF("%-60s= %d", "\tbufferDeviceAddressMultiDevice",                     characteristics->buffer_device_address_multi_device);
    LOGF("%-60s= %d", "\tvulkanMemoryModel",                                  characteristics->vulkan_memory_model);
    LOGF("%-60s= %d", "\tvulkanMemoryModelDeviceScope",                       characteristics->vulkan_memory_model_device_scope);
    LOGF("%-60s= %d", "\tvulkanMemoryModelAvailabilityVisibilityChains",      characteristics->vulkan_memory_model_availability_visibility_chains);
    LOGF("%-60s= %d", "\tshaderOutputViewportIndex",                          characteristics->shader_output_viewport_index);
    LOGF("%-60s= %d", "\tshaderOutputLayer",                                  characteristics->shader_output_layer);
    LOGF("%-60s= %d", "\tsubgroupBroadcastDynamicId",                         characteristics->subgroup_broadcast_dynamic_id);
    LOGF("%-60s", "} // VkPhysicalDeviceVulkan12Features");

    LOGF("%-60s", "VkPhysicalDeviceVulkan13Features {");
    LOGF("%-60s= %d", "\trobustImageAccess",                                  characteristics->robust_image_access);
    LOGF("%-60s= %d", "\tinlineUniformBlock",                                 characteristics->inline_uniform_block);
    LOGF("%-60s= %d", "\tdescriptorBindingInlineUniformBlockUpdateAfterBind", characteristics->descriptor_binding_inline_uniform_block_update_after_bind);
    LOGF("%-60s= %d", "\tpipelineCreationCacheControl",                       characteristics->pipeline_creation_cache_control);
    LOGF("%-60s= %d", "\tprivateData",                                        characteristics->private_data);
    LOGF("%-60s= %d", "\tshaderDemoteToHelperInvocation",                     characteristics->shader_demote_to_helper_invocation);
    LOGF("%-60s= %d", "\tshaderTerminateInvocation",                          characteristics->shader_terminate_invocation);
    LOGF("%-60s= %d", "\tsubgroupSizeControl",                                characteristics->subgroup_size_control);
    LOGF("%-60s= %d", "\tcomputeFullSubgroups",                               characteristics->compute_full_subgroups);
    LOGF("%-60s= %d", "\tsynchronization2",                                   characteristics->synchronization2);
    LOGF("%-60s= %d", "\ttextureCompressionASTC_HDR",                         characteristics->texture_compression_astc_hdr);
    LOGF("%-60s= %d", "\tshaderZeroInitializeWorkgroupMemory",                characteristics->shader_zero_initialize_workgroup_memory);
    LOGF("%-60s= %d", "\tdynamicRendering",                                   characteristics->dynamic_rendering);
    LOGF("%-60s= %d", "\tshaderIntegerDotProduct",                            characteristics->shader_integer_dot_product);
    LOGF("%-60s= %d", "\tmaintenance4",                                       characteristics->maintenance4);
    LOGF("%-60s", "} // VkPhysicalDeviceVulkan13Features");

    LOGF("%-60s", "Extensions-----------");
    for (Vdusize i = 0; i < characteristics->num_extensions; ++i)
        LOGF("%-60s", characteristics->extensions[i]);

    LOGF("%-60s", "Queue Setups---------");
    for (Vdusize i = 0; i < characteristics->num_queue_setups; ++i) {
        LOGF("[%zu].required_count: %u", i, characteristics->queue_setups[i].required_count);
        LOGF("[%zu].present_count:  %u", i, characteristics->queue_setups[i].present_count);
        LOGF("[%zu].cached_index:   %d", i, characteristics->queue_setups[i].cached_index);
        LOGF("%s", "                      Graphics Compute Transfer Present");
        LOGF("[%zu].capabilities:     %d        %d       %d        %d",
            i,
            characteristics->queue_setups[i].capabilities & VD_VK_QUEUE_CAPABILITIES_GRAPHICS ? 1 : 0,
            characteristics->queue_setups[i].capabilities & VD_VK_QUEUE_CAPABILITIES_COMPUTE  ? 1 : 0,
            characteristics->queue_setups[i].capabilities & VD_VK_QUEUE_CAPABILITIES_TRANSFER ? 1 : 0,
            characteristics->queue_setups[i].capabilities & VD_VK_QUEUE_CAPABILITIES_PRESENT  ? 1 : 0);
    }

    LOGF("%-60s", "Surface Formats------");
    for (Vdusize i = 0; i < characteristics->num_surface_formats; ++i) {
        LOGF("%-50s %30s",
            string_VkFormat(characteristics->surface_formats[i].format),
            string_VkColorSpaceKHR(characteristics->surface_formats[i].colorSpace));
    }
}

void vd_vk_create_device_and_queues(VdArena *temp, VdVkCreateDeviceAndQueuesInfo *info)
{
    VD_ASSERT(info->characteristics->num_queue_setups == info->result.num_queue_groups);

    Vdu32 num_queue_groups = (Vdu32)info->characteristics->num_queue_setups;

    // Create arrays for VkDeviceQueueCreateInfo
    VkDeviceQueueCreateInfo *queue_create_infos = ARENA_PUSH_ARRAY(temp, VkDeviceQueueCreateInfo, num_queue_groups); 
    for (Vdu32 i = 0; i < num_queue_groups; ++i) {
        VD_ASSERT(info->characteristics->queue_setups[i].required_count == info->result.queue_groups[i].queue_count);
        VD_ASSERT(info->characteristics->queue_setups[i].cached_index >= 0);

        Vdu32 queue_count = info->characteristics->queue_setups[i].required_count;

        float *priorities = ARENA_PUSH_ARRAY(temp, float, queue_count);
        for (Vdu32 j = 0; j < queue_count; ++j) {
            priorities[j] = 1.0f;
        }

        queue_create_infos[i] = (VkDeviceQueueCreateInfo) {
            .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex   = info->characteristics->queue_setups[i].cached_index,
            .queueCount         = queue_count,
            .pQueuePriorities   = priorities,
        };
    }

    VkPhysicalDeviceVulkan12Features features12 = {
        .sType                                              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .samplerMirrorClampToEdge                           = info->characteristics->sampler_mirror_clamp_to_edge,
        .drawIndirectCount                                  = info->characteristics->draw_indirect_count,
        .storageBuffer8BitAccess                            = info->characteristics->storage_buffer8_bit_access,
        .uniformAndStorageBuffer8BitAccess                  = info->characteristics->uniform_and_storage_buffer8_bit_access,
        .storagePushConstant8                               = info->characteristics->storage_push_constant8,
        .shaderBufferInt64Atomics                           = info->characteristics->shader_buffer_int64_atomics,
        .shaderSharedInt64Atomics                           = info->characteristics->shader_shared_int64_atomics,
        .shaderFloat16                                      = info->characteristics->shader_float16,
        .shaderInt8                                         = info->characteristics->shader_int8,
        .descriptorIndexing                                 = info->characteristics->descriptor_indexing,
        .shaderInputAttachmentArrayDynamicIndexing          = info->characteristics->shader_input_attachment_array_dynamic_indexing,
        .shaderUniformTexelBufferArrayDynamicIndexing       = info->characteristics->shader_uniform_texel_buffer_array_dynamic_indexing,
        .shaderStorageTexelBufferArrayDynamicIndexing       = info->characteristics->shader_storage_texel_buffer_array_dynamic_indexing,
        .shaderUniformBufferArrayNonUniformIndexing         = info->characteristics->shader_uniform_buffer_array_non_uniform_indexing,
        .shaderSampledImageArrayNonUniformIndexing          = info->characteristics->shader_sampled_image_array_non_uniform_indexing,
        .shaderStorageBufferArrayNonUniformIndexing         = info->characteristics->shader_storage_buffer_array_non_uniform_indexing,
        .shaderStorageImageArrayNonUniformIndexing          = info->characteristics->shader_storage_image_array_non_uniform_indexing,
        .shaderInputAttachmentArrayNonUniformIndexing       = info->characteristics->shader_input_attachment_array_non_uniform_indexing,
        .shaderUniformTexelBufferArrayNonUniformIndexing    = info->characteristics->shader_uniform_texel_buffer_array_non_uniform_indexing,
        .shaderStorageTexelBufferArrayNonUniformIndexing    = info->characteristics->shader_storage_texel_buffer_array_non_uniform_indexing,
        .descriptorBindingUniformBufferUpdateAfterBind      = info->characteristics->descriptor_binding_uniform_buffer_update_after_bind,
        .descriptorBindingSampledImageUpdateAfterBind       = info->characteristics->descriptor_binding_sampled_image_update_after_bind,
        .descriptorBindingStorageImageUpdateAfterBind       = info->characteristics->descriptor_binding_storage_image_update_after_bind,
        .descriptorBindingStorageBufferUpdateAfterBind      = info->characteristics->descriptor_binding_storage_buffer_update_after_bind,
        .descriptorBindingUniformTexelBufferUpdateAfterBind = info->characteristics->descriptor_binding_uniform_texel_buffer_update_after_bind,
        .descriptorBindingStorageTexelBufferUpdateAfterBind = info->characteristics->descriptor_binding_storage_texel_buffer_update_after_bind,
        .descriptorBindingUpdateUnusedWhilePending          = info->characteristics->descriptor_binding_update_unused_while_pending,
        .descriptorBindingPartiallyBound                    = info->characteristics->descriptor_binding_partially_bound,
        .descriptorBindingVariableDescriptorCount           = info->characteristics->descriptor_binding_variable_descriptor_count,
        .runtimeDescriptorArray                             = info->characteristics->runtime_descriptor_array,
        .samplerFilterMinmax                                = info->characteristics->sampler_filter_minmax,
        .scalarBlockLayout                                  = info->characteristics->scalar_block_layout,
        .imagelessFramebuffer                               = info->characteristics->imageless_framebuffer,
        .uniformBufferStandardLayout                        = info->characteristics->uniform_buffer_standard_layout,
        .shaderSubgroupExtendedTypes                        = info->characteristics->shader_subgroup_extended_types,
        .separateDepthStencilLayouts                        = info->characteristics->separate_depth_stencil_layouts,
        .hostQueryReset                                     = info->characteristics->host_query_reset,
        .timelineSemaphore                                  = info->characteristics->timeline_semaphore,
        .bufferDeviceAddress                                = info->characteristics->buffer_device_address,
        .bufferDeviceAddressCaptureReplay                   = info->characteristics->buffer_device_address_capture_replay,
        .bufferDeviceAddressMultiDevice                     = info->characteristics->buffer_device_address_multi_device,
        .vulkanMemoryModel                                  = info->characteristics->vulkan_memory_model,
        .vulkanMemoryModelDeviceScope                       = info->characteristics->vulkan_memory_model_device_scope,
        .vulkanMemoryModelAvailabilityVisibilityChains      = info->characteristics->vulkan_memory_model_availability_visibility_chains,
        .shaderOutputViewportIndex                          = info->characteristics->shader_output_viewport_index,
        .shaderOutputLayer                                  = info->characteristics->shader_output_layer,
        .subgroupBroadcastDynamicId                         = info->characteristics->subgroup_broadcast_dynamic_id,
    };

    VkPhysicalDeviceVulkan13Features features13 = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &features12,
        .robustImageAccess                                  = info->characteristics->robust_image_access,
        .inlineUniformBlock                                 = info->characteristics->inline_uniform_block,
        .descriptorBindingInlineUniformBlockUpdateAfterBind = info->characteristics->descriptor_binding_inline_uniform_block_update_after_bind,
        .pipelineCreationCacheControl                       = info->characteristics->pipeline_creation_cache_control,
        .privateData                                        = info->characteristics->private_data,
        .shaderDemoteToHelperInvocation                     = info->characteristics->shader_demote_to_helper_invocation,
        .shaderTerminateInvocation                          = info->characteristics->shader_terminate_invocation,
        .subgroupSizeControl                                = info->characteristics->subgroup_size_control,
        .computeFullSubgroups                               = info->characteristics->compute_full_subgroups,
        .synchronization2                                   = info->characteristics->synchronization2,
        .textureCompressionASTC_HDR                         = info->characteristics->texture_compression_astc_hdr,
        .shaderZeroInitializeWorkgroupMemory                = info->characteristics->shader_zero_initialize_workgroup_memory,
        .dynamicRendering                                   = info->characteristics->dynamic_rendering,
        .shaderIntegerDotProduct                            = info->characteristics->shader_integer_dot_product,
        .maintenance4                                       = info->characteristics->maintenance4,
    };

    VkPhysicalDeviceFeatures2 features = {
        .sType                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext                  = &features13,
        .features               = {
            .robustBufferAccess                      = info->characteristics->robust_buffer_access,
            .fullDrawIndexUint32                     = info->characteristics->full_draw_index_uint32,
            .imageCubeArray                          = info->characteristics->image_cube_array,
            .independentBlend                        = info->characteristics->independent_blend,
            .geometryShader                          = info->characteristics->geometry_shader,
            .tessellationShader                      = info->characteristics->tessellation_shader,
            .sampleRateShading                       = info->characteristics->sample_rate_shading,
            .dualSrcBlend                            = info->characteristics->dual_src_blend,
            .logicOp                                 = info->characteristics->logic_op,
            .multiDrawIndirect                       = info->characteristics->multi_draw_indirect,
            .drawIndirectFirstInstance               = info->characteristics->draw_indirect_first_instance,
            .depthClamp                              = info->characteristics->depth_clamp,
            .depthBiasClamp                          = info->characteristics->depth_bias_clamp,
            .fillModeNonSolid                        = info->characteristics->fill_mode_non_solid,
            .depthBounds                             = info->characteristics->depth_bounds,
            .wideLines                               = info->characteristics->wide_lines,
            .largePoints                             = info->characteristics->large_points,
            .alphaToOne                              = info->characteristics->alpha_to_one,
            .multiViewport                           = info->characteristics->multi_viewport,
            .samplerAnisotropy                       = info->characteristics->sampler_anisotropy,
            .textureCompressionETC2                  = info->characteristics->texture_compression_etc2,
            .textureCompressionASTC_LDR              = info->characteristics->texture_compression_astc_ldr,
            .textureCompressionBC                    = info->characteristics->texture_compression_bc,
            .occlusionQueryPrecise                   = info->characteristics->occlusion_query_precise,
            .pipelineStatisticsQuery                 = info->characteristics->pipeline_statistics_query,
            .vertexPipelineStoresAndAtomics          = info->characteristics->vertex_pipeline_stores_and_atomics,
            .fragmentStoresAndAtomics                = info->characteristics->fragment_stores_and_atomics,
            .shaderTessellationAndGeometryPointSize  = info->characteristics->shader_tessellation_and_geometry_point_size,
            .shaderImageGatherExtended               = info->characteristics->shader_image_gather_extended,
            .shaderStorageImageExtendedFormats       = info->characteristics->shader_storage_image_extended_formats,
            .shaderStorageImageMultisample           = info->characteristics->shader_storage_image_multisample,
            .shaderStorageImageReadWithoutFormat     = info->characteristics->shader_storage_image_read_without_format,
            .shaderStorageImageWriteWithoutFormat    = info->characteristics->shader_storage_image_write_without_format,
            .shaderUniformBufferArrayDynamicIndexing = info->characteristics->shader_uniform_buffer_array_dynamic_indexing,
            .shaderSampledImageArrayDynamicIndexing  = info->characteristics->shader_sampled_image_array_dynamic_indexing,
            .shaderStorageBufferArrayDynamicIndexing = info->characteristics->shader_storage_buffer_array_dynamic_indexing,
            .shaderStorageImageArrayDynamicIndexing  = info->characteristics->shader_storage_image_array_dynamic_indexing,
            .shaderClipDistance                      = info->characteristics->shader_clip_distance,
            .shaderCullDistance                      = info->characteristics->shader_cull_distance,
            .shaderFloat64                           = info->characteristics->shader_float64,
            .shaderInt64                             = info->characteristics->shader_int64,
            .shaderInt16                             = info->characteristics->shader_int16,
            .shaderResourceResidency                 = info->characteristics->shader_resource_residency,
            .shaderResourceMinLod                    = info->characteristics->shader_resource_min_lod,
            .sparseBinding                           = info->characteristics->sparse_binding,
            .sparseResidencyBuffer                   = info->characteristics->sparse_residency_buffer,
            .sparseResidencyImage2D                  = info->characteristics->sparse_residency_image2_d,
            .sparseResidencyImage3D                  = info->characteristics->sparse_residency_image3_d,
            .sparseResidency2Samples                 = info->characteristics->sparse_residency2_samples,
            .sparseResidency4Samples                 = info->characteristics->sparse_residency4_samples,
            .sparseResidency8Samples                 = info->characteristics->sparse_residency8_samples,
            .sparseResidency16Samples                = info->characteristics->sparse_residency16_samples,
            .sparseResidencyAliased                  = info->characteristics->sparse_residency_aliased,
            .variableMultisampleRate                 = info->characteristics->variable_multisample_rate,
            .inheritedQueries                        = info->characteristics->inherited_queries,
        },
    };

    VD_VK_CHECK(vkCreateDevice(info->characteristics->physical_device, & (VkDeviceCreateInfo) {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount    = (Vdu32)info->characteristics->num_queue_setups,
        .pQueueCreateInfos       = queue_create_infos,
        .enabledExtensionCount   = (Vdu32)info->characteristics->num_extensions,
        .ppEnabledExtensionNames = (const char * const *)info->characteristics->extensions,
        .pNext                   = &features,
    }, 0, info->result.device));

    for (Vdu32 i = 0; i < num_queue_groups; ++i) {
        for (Vdu32 q = 0; q < info->result.queue_groups[i].queue_count; ++q) {
            vkGetDeviceQueue(
                *info->result.device,
                info->characteristics->queue_setups[i].cached_index, 
                q,
                &info->result.queue_groups[i].queues[q]);
        }
    }
}

void vd_vk_create_swapchain_and_fetch_images(VdArena *a, VdVkCreateSwapchainAndFetchImagesInfo *info)
{
    VkSurfaceCapabilitiesKHR surface_caps;

    // Query swapchain capabilities
    VD_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->physical_device, info->surface, &surface_caps));

    Vdu32 result_image_count = surface_caps.minImageCount + 1;
    if (surface_caps.maxImageCount > 0 && surface_caps.maxImageCount < result_image_count) {
        result_image_count = surface_caps.maxImageCount;
    }

    VD_VK_CHECK(vkCreateSwapchainKHR(info->device, &(VkSwapchainCreateInfoKHR) {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface               = info->surface,
        .minImageCount         = result_image_count,
        .imageFormat           = info->surface_format.format,
        .imageColorSpace       = info->surface_format.colorSpace,
        .imageExtent           = info->extent,
        .imageArrayLayers      = 1,
        .imageUsage            = info->image_usage,
        // @note(mdodis): This could be VK_SHARING_MODE_CONCURRENT if present and graphics queue families are the same
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = NULL,
        .preTransform          = surface_caps.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        // @todo(mdodis): pick best present mode based on user preference (or if on mobile use the same)
        .presentMode           = VK_PRESENT_MODE_FIFO_KHR,
        .clipped               = VK_TRUE,
        .oldSwapchain          = VK_NULL_HANDLE,
    }, 0, info->result.swapchain));

    Vdu32 num_swapchain_images;
    VD_VK_CHECK(vkGetSwapchainImagesKHR(info->device, *info->result.swapchain, &num_swapchain_images, 0));
    VkImage *swapchain_images = VD_ARENA_PUSH_ARRAY(a, VkImage, num_swapchain_images);
    VD_VK_CHECK(vkGetSwapchainImagesKHR(info->device, *info->result.swapchain, &num_swapchain_images, swapchain_images));

    *info->result.num_images = num_swapchain_images;
    *info->result.images = swapchain_images;
}

void vd_vk_create_image_views(VdArena *a, VdVkCreateImageViewsInfo *info)
{
    VkImageViewCreateInfo default_create_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = info->image_format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    VkImageViewCreateInfo *create_info = info->override_info;    
    if (create_info == NULL) {
        create_info = &default_create_info;
    }

    VkImageView *views = VD_ARENA_PUSH_ARRAY(a, VkImageView, info->num_images);

    for (Vdu32 i = 0; i < info->num_images; ++i) {
        create_info->image = info->images[i];
        VD_VK_CHECK(vkCreateImageView(info->device, create_info, 0, &views[i]));
    }

    *info->result.image_views = views;
}

void vd_vk_cmd_image_transition(VkCommandBuffer cmd, VdVkCmdImageTransitionInfo *info)
{
    vkCmdPipelineBarrier(
        cmd,
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        0,                           // VkDependencyFlagBits
        0,                           // memoryBarrierCount,
        0,                           // pMemoryBarriers,
        0,                           // bufferMemoryBarrierCount,
        0,                           // pBufferMemoryBarriers,
        1,                           // imageMemoryBarrierCount,
        (VkImageMemoryBarrier []) {
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .srcAccessMask       = VK_ACCESS_MEMORY_WRITE_BIT,
                .dstAccessMask       = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT,
                .oldLayout           = info->current_layout,
                .newLayout           = info->target_layout,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image               = info->image,
                .subresourceRange    = {
                    .aspectMask      = info->aspect,
                    .baseMipLevel    = info->base_mip_level,
                    .levelCount      = info->level_count,
                    .baseArrayLayer  = info->base_array_layer,
                    .layerCount      = info->layer_count,
                },
            },
        });
}

void vd_vk_destroy_image_views(VkDevice device, Vdu32 num_image_views, VkImageView *image_views)
{
    for (Vdu32 i = 0; i < num_image_views; ++i) {
        vkDestroyImageView(device, image_views[i], 0);
    }
}

void vd_vk_destroy_framebuffers(VkDevice device, Vdu32 num_framebuffers, VkFramebuffer *framebuffers)
{
    for (Vdu32 i = 0; i < num_framebuffers; ++i) {
        vkDestroyFramebuffer(device, framebuffers[i], 0);
    }
}

void vd_vk_destroy_semaphores(VkDevice device, Vdu32 num_semaphores, VkSemaphore *semaphores)
{
    for (Vdu32 i = 0; i < num_semaphores; ++i) {
        vkDestroySemaphore(device, semaphores[i], 0);
    }
}

/* ----IMMEDIATE COMMANDS IMPL--------------------------------------------------------------------------------------- */
void vd_vk_immediate_commands_init(VdVkImmediateCommands *cmds, VdVkImmediateCommandsInitInfo *info)
{
    cmds->device = info->device;
    cmds->recording = VD_FALSE;
    cmds->queue = info->queue;

    VD_VK_CHECK(vkCreateCommandPool(info->device, & (VkCommandPoolCreateInfo) {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = info->queue_family_index,
    }, 0, &cmds->pool));

    VD_VK_CHECK(vkAllocateCommandBuffers(info->device, & (VkCommandBufferAllocateInfo) {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = cmds->pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    }, &cmds->buffer));

    VD_VK_CHECK(vkCreateFence(info->device, & (VkFenceCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = 0,
    }, 0, &cmds->fence));
}

VkCommandBuffer vd_vk_immediate_commands_begin(VdVkImmediateCommands *cmds)
{
    if (cmds->recording) {
        VD_ASSERT(0 && "Cannot call vk_immediate_commands_begin without having called vk_immediate_commands_end in the past!");
    }
    VD_VK_CHECK(vkResetFences(cmds->device, 1, &cmds->fence));
    VD_VK_CHECK(vkResetCommandBuffer(cmds->buffer, 0));

    VD_VK_CHECK(vkBeginCommandBuffer(cmds->buffer, & (VkCommandBufferBeginInfo) {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    }));

    cmds->recording = VD_TRUE;
    return cmds->buffer;
}

void vd_vk_immediate_commands_end(VdVkImmediateCommands *cmds)
{
    if (!cmds->recording) {
        VD_ASSERT(0 && "Cannot call vk_immediate_commands_end without having called vk_immediate_commands_begin in the past!");
    }

    VD_VK_CHECK(vkEndCommandBuffer(cmds->buffer));

    VD_VK_CHECK(vkQueueSubmit(cmds->queue, 1, & (VkSubmitInfo) {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount   = 1,
        .pCommandBuffers       = (VkCommandBuffer[]) {
            cmds->buffer,
        },
    }, cmds->fence));

    VD_VK_CHECK(vkWaitForFences(cmds->device, 1, &cmds->fence, 1, 99999999));

    cmds->recording = VD_FALSE;
}

void vd_vk_immediate_commands_deinit(VdVkImmediateCommands *cmds)
{
    vkDestroyCommandPool(cmds->device, cmds->pool, 0);
    vkDestroyFence(cmds->device, cmds->fence, 0);
}

/* ----AMD VMA TRACKING IMPL----------------------------------------------------------------------------------------- */
#if VD_VK_VMA_TRACKING

#ifndef VD_VK_VMA_TRACKING_MAX_FILEPATH_CUSTOM
#define VD_VK_VMA_TRACKING_MAX_FILEPATH_CUSTOM 0
#endif // !VD_VK_VMA_TRACKING_MAX_FILEPATH_CUSTOM

#if !VD_VK_VMA_TRACKING_MAX_FILEPATH_CUSTOM
#define VD_VK_VMA_MAX_FILEPATH 128
#endif // !VD_VK_VMA_TRACKING_MAX_FILEPATH_CUSTOM

#ifndef VD_VK_VMA_TRACKING_MAX_ENTRIES_CUSTOM
#define VD_VK_VMA_TRACKING_MAX_ENTRIES_CUSTOM 0
#endif // !VD_VK_VMA_TRACKING_MAX_ENTRIES_CUSTOM

#if !VD_VK_VMA_TRACKING_MAX_ENTRIES_CUSTOM
#define VD_VK_VMA_MAX_ENTRIES 1024
#endif // !VD_VK_VMA_TRACKING_MAX_ENTRIES_CUSTOM

typedef struct VdVk__VmaTrackingEntry VdVk__VmaTrackingEntry;

struct VdVk__VmaTrackingEntry {
    char                    filepath[VD_VK_VMA_MAX_FILEPATH];
    Vdi32                 line;
    Vdb32                 used;
    VdVk__VmaTrackingEntry *n;
    VdVk__VmaTrackingEntry *p;
};

struct {
    VdVk__VmaTrackingEntry     sentinel;
    VdVk__VmaTrackingEntry     *entries;
    Vdusize                   num_used;
} VdVk___Vma_Tracking = {
    .entries = 0,
    .num_used = 0,
};

void vd_vk__vma_track_allocation(VmaAllocator allocator, VmaAllocation allocation, const char *file, int filelen, int line)
{
    VD_UNUSED(allocator);
    VD_UNUSED(allocation);
    VD_TODO();

    const Vdusize allocation_size = sizeof(VdVk__VmaTrackingEntry) * VD_VK_VMA_MAX_ENTRIES;
    if (VdVk___Vma_Tracking.entries == 0) {
        VdVk___Vma_Tracking.sentinel.n = &VdVk___Vma_Tracking.sentinel;
        VdVk___Vma_Tracking.sentinel.p = &VdVk___Vma_Tracking.sentinel;
        VdVk___Vma_Tracking.entries = VD_MEMSET(VD_MALLOC(allocation_size), 0, allocation_size);
    }

    VD_ASSERT((VdVk___Vma_Tracking.num_used < VD_VK_VMA_MAX_ENTRIES) &&
              (VdVk___Vma_Tracking.sentinel.n != VdVk___Vma_Tracking.sentinel.p));

    // Allocate the entry
    VdVk__VmaTrackingEntry *entry;

    if (VdVk___Vma_Tracking.num_used < VD_VK_VMA_MAX_ENTRIES) {
        // We can just bump the pointer and then allocate
        Vdu64 entry_index = VdVk___Vma_Tracking.num_used++;
        entry = &VdVk___Vma_Tracking.entries[entry_index];
    } else {
        // We can get one from the free list
        VdVk__VmaTrackingEntry *S = &VdVk___Vma_Tracking.sentinel;
        VdVk__VmaTrackingEntry *R = S->n;
        VdVk__VmaTrackingEntry *N = R->n;

        S->n = N;
        N->p = S;

        entry = R;
    }

    // Populate the entry
    VD_MEMCPY(entry->filepath, file, (VD_VK_VMA_MAX_FILEPATH > filelen) ? filelen : (VD_VK_VMA_MAX_FILEPATH - 1));
    entry->line = line;
    entry->used = VD_TRUE;
}

void vd_vk__vma_release_allocation(VmaAllocator allocator, VmaAllocation allocation)
{
    VmaAllocationInfo info;
    vmaGetAllocationInfo(allocator, allocation, &info);
    VdVk__VmaTrackingEntry *entry = (VdVk__VmaTrackingEntry*)info.pUserData;
    entry->used = VD_FALSE;

    VdVk__VmaTrackingEntry *S = &VdVk___Vma_Tracking.sentinel;
    VdVk__VmaTrackingEntry *R = entry;
    VdVk__VmaTrackingEntry *N = S->n;

    N->p = R;
    R->n = N;
    S->n = R;
}

void vd_vk__vma_check_allocations(void) {
    VdVk__VmaTrackingEntry *S = &VdVk___Vma_Tracking.sentinel;
    VdVk__VmaTrackingEntry *n = VdVk___Vma_Tracking.sentinel.n;
    while (n != S) {
        ERRF("VMA: Failed to release allocation at: %s:%d", n->filepath, n->line);
        n = n->n;
    }    
}

#endif // VD_VK_VMA_TRACKING

/* ----GROWABLE DESCRIPTOR ALLOCATOR IMPL---------------------------------------------------------------------------- */

static VdVk__GrowableDescriptorAllocatorPool* vd_vk__growable_descriptor_allocator_grab_pool(VdVkGrowableDescriptorAllocator *desc_alloc);

static VdVkGrowableDescriptorAllocatorPoolSizeRatio Vd__Growable_Descriptor_Allocator_Default_Pool_Sizes[] = {
    { VK_DESCRIPTOR_TYPE_SAMPLER,                0.5f},
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f},
    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          4.f},
    { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1.f},
    { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1.f},
    { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1.f},
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         2.f},
    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         2.f},
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f},
    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f},
    { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       0.5f},
};

void vd_vk_growable_descriptor_allocator_init(VdVkGrowableDescriptorAllocator *desc_alloc, VdVkGrowableDescriptorAllocatorInitInfo *info)
{
    desc_alloc->arena        = info->arena;
    desc_alloc->device       = info->device;
    desc_alloc->current_pool = 0;

    vd_dlist_init(&desc_alloc->used_pool_list);
    vd_dlist_init(&desc_alloc->free_pool_list);

    if (info->override_pool_sizes != 0) {
        desc_alloc->num_ratios = info->num_override_pool_sizes;
        desc_alloc->ratios     = info->override_pool_sizes;
    } else {
        desc_alloc->num_ratios = VD_ARRAY_COUNT(Vd__Growable_Descriptor_Allocator_Default_Pool_Sizes);
        desc_alloc->ratios     = Vd__Growable_Descriptor_Allocator_Default_Pool_Sizes;
    }
}

VkResult vd_vk_growable_descriptor_allocator_get(VdVkGrowableDescriptorAllocator *desc_alloc, VkDescriptorSetLayout layout, VkDescriptorSet *set)
{
    if (desc_alloc->current_pool == 0) {
        VdVk__GrowableDescriptorAllocatorPool *p;
        p = vd_vk__growable_descriptor_allocator_grab_pool(desc_alloc);
        desc_alloc->current_pool = p;
        vd_dlist_append(&desc_alloc->used_pool_list, &p->node);
    }

    VkResult result = vkAllocateDescriptorSets(desc_alloc->device, & (VkDescriptorSetAllocateInfo) {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = desc_alloc->current_pool->pool,
        .descriptorSetCount = 1,
        .pSetLayouts        = &layout,
    }, set);

    if ((result == VK_ERROR_FRAGMENTED_POOL) || (result == VK_ERROR_OUT_OF_POOL_MEMORY)) {

        VdVk__GrowableDescriptorAllocatorPool *p;
        p = vd_vk__growable_descriptor_allocator_grab_pool(desc_alloc);
        desc_alloc->current_pool = p;
        vd_dlist_append(&desc_alloc->used_pool_list, &p->node);

        result = vkAllocateDescriptorSets(desc_alloc->device, & (VkDescriptorSetAllocateInfo) {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool     = desc_alloc->current_pool->pool,
            .descriptorSetCount = 1,
            .pSetLayouts        = &layout,
        }, set);

        return result;
    }

    return result;
}

void vd_vk_growable_descriptor_allocator_reset(VdVkGrowableDescriptorAllocator *desc_alloc)
{
    VD_DLIST_FOR_EACH(&desc_alloc->used_pool_list, n) {
        VdVk__GrowableDescriptorAllocatorPool *p = VD_CONTAINER_OF(n, VdVk__GrowableDescriptorAllocatorPool, node);

        VD_VK_CHECK(vkResetDescriptorPool(desc_alloc->device, p->pool, 0));
    }

    while (!vd_dlist_is_empty(&desc_alloc->used_pool_list)) {
        VdDListNode *n = vd_dlist_rm_first(&desc_alloc->used_pool_list);
        vd_dlist_append(&desc_alloc->free_pool_list, n);
    }

    desc_alloc->current_pool = 0;
}

void vd_vk_growable_descriptor_allocator_deinit(VdVkGrowableDescriptorAllocator *desc_alloc)
{
    VD_DLIST_FOR_EACH(&desc_alloc->used_pool_list, n) {
        VdVk__GrowableDescriptorAllocatorPool *p = VD_CONTAINER_OF(n, VdVk__GrowableDescriptorAllocatorPool, node);
        vkDestroyDescriptorPool(desc_alloc->device, p->pool, 0);
    }

    VD_DLIST_FOR_EACH(&desc_alloc->free_pool_list, n) {
        VdVk__GrowableDescriptorAllocatorPool *p = VD_CONTAINER_OF(n, VdVk__GrowableDescriptorAllocatorPool, node);
        vkDestroyDescriptorPool(desc_alloc->device, p->pool, 0);
    }
}

static VdVk__GrowableDescriptorAllocatorPool *vd_vk__growable_descriptor_allocator_grab_pool(VdVkGrowableDescriptorAllocator *desc_alloc)
{
    // Try to get one from 
    VdDListNode *result = vd_dlist_rm_first(&desc_alloc->free_pool_list);
    if (result != 0) {
        return VD_CONTAINER_OF(result, VdVk__GrowableDescriptorAllocatorPool, node);
    }

    // Create a pool
    VdVk__GrowableDescriptorAllocatorPool *pool = VD_ARENA_PUSH_STRUCT(desc_alloc->arena, VdVk__GrowableDescriptorAllocatorPool);
    vd_dlist_node_init(&pool->node);

    VdArenaSave save = vd_arena_save(desc_alloc->arena);

    Vdu32 num_pool_sizes = (Vdu32)desc_alloc->num_ratios;
    VkDescriptorPoolSize *pool_sizes = VD_ARENA_PUSH_ARRAY(desc_alloc->arena, VkDescriptorPoolSize, desc_alloc->num_ratios);

    // @todo(mdodis): Customize this in the future
    const Vdu32 max_sets = 1000;

    for (Vdu32 i = 0; i < num_pool_sizes; ++i) {
        pool_sizes[i]        = (VkDescriptorPoolSize) {
            .type            = desc_alloc->ratios[i].type,
            .descriptorCount = (Vdu32)(desc_alloc->ratios[i].ratio * max_sets),
        };
    }

    VD_VK_CHECK(vkCreateDescriptorPool(desc_alloc->device, & (VkDescriptorPoolCreateInfo) {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags         = 0,
        .maxSets       = max_sets,
        .poolSizeCount = num_pool_sizes,
        .pPoolSizes    = pool_sizes,
    }, 0, &pool->pool));

    vd_arena_restore(save);

    return pool;
}

#endif // VD_VK_IMPL