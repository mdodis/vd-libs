#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DBG_BREAK() abort()
#define VK_CHECK(expr) do { \
        VkResult __result__ = (expr); \
        if (__result__ != VK_SUCCESS) { \
            printf("Vulkan Assertion Failed: '" #expr "' at %s %d\t%s\n", __FILE__, __LINE__, string_VkResult(__result__));\
            abort(); \
        }\
    } while(0)

#define ALLOC_ARRAY(typ, cnt) (typ*)malloc(sizeof(typ) * cnt)

static void *spriv_load(const char *path, size_t *size);
static void swapchain_image_barrier(VkCommandBuffer cmd, uint32_t image_index,
                                    uint32_t src_queue_family, uint32_t dst_queue_family,
                                    VkImageLayout old_layout, VkImageLayout new_layout,
                                    VkAccessFlags2 src_access, VkAccessFlags2 dst_access,
                                    VkPipelineStageFlags2 src_stage, VkPipelineStageFlags2 dst_stage);
static VkExtent2D create_swapchain_and_image_views();
static VkBool32 debug_utils_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                               VkDebugUtilsMessageTypeFlagsEXT message_types,
                                               const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                               void *usr_data);

typedef struct {
    VkCommandBuffer                             cmd_buffer;
    VkSemaphore                                 sem_present_complete;
    VkFence                                     fnc_render_complete;
} VkFrameData;

typedef struct {
    VkInstance                                  instance;
    VkPhysicalDevice                            physical_device;

    PFN_vkCreateDebugUtilsMessengerEXT          debug_utils_create_messenger;
    PFN_vkDestroyDebugUtilsMessengerEXT         debug_utils_destroy_messenger;
    VkDebugUtilsMessengerEXT                    debug_utils_messenger;

    VkSurfaceKHR                                surface;
    VkSurfaceFormatKHR                          surface_format;

    VkSwapchainKHR                              swapchain;
    uint32_t                                    num_swapchain_images;
    uint32_t                                    min_swapchain_image_count;
    VkPresentModeKHR                            present_mode;
    VkImage                                     *swapchain_images;
    VkImageView                                 *swapchain_image_views;

    VkDevice                                    device;

    uint32_t                                    present_queue_family;
    VkQueue                                     present_queue;

    uint32_t                                    graphics_queue_family;
    VkQueue                                     graphics_queue;

    VkCommandPool                               cmd_pool;

    uint32_t                                    num_frames;
    uint32_t                                    curr_frame;
    VkFrameData                                 *frames;
    VkSemaphore                                 *sems_render_complete;

    VkShaderModule                              shader_module;
    VkPipelineLayout                            pipeline_layout;
    VkPipeline                                  graphics_pipeline;
} Vk;

static Vk VK = {0};

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .api = VD_FW_GRAPHICS_API_CUSTOM,
        .window_options = {
            .borderless = 0,
            .block_while_sizing = 0,
        }
    });

    VkExtent2D window_size;
    {
        int w, h;
        vd_fw_get_size(&w, &h);
        window_size.width = (uint32_t)w;
        window_size.height = (uint32_t)h;
    }

    VkClearColorValue clear_color_value = {0.2f, 0.1f, 0.7f, 1.f};
    uint32_t api_version = VK_MAKE_VERSION(1, 3, 0);
    int frames_in_flight = 2;

    // -----------------------------------------------------------------------------------------------------------------
    // Vulkan Initialization
    {
        const char *required_instance_extensions[] = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
            VK_KHR_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
            VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
        };
        int num_required_instance_extensions = sizeof(required_instance_extensions) / sizeof(required_instance_extensions[0]);

        const char *required_instance_layers[] = {
            "VK_LAYER_KHRONOS_validation",
        };

        int num_required_instance_layers = sizeof(required_instance_layers) / sizeof(required_instance_layers[0]);

        const char *required_device_extensions[] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
            VK_KHR_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME,
        };
        int num_required_device_extensions = sizeof(required_device_extensions) / sizeof(required_device_extensions[0]);

        // -------------------------------------------------------------------------------------------------------------
        // Instance Extensions
        int num_all_instance_extensions = 0;
        const char *all_instance_extensions[12];

        int num_wsi_instance_extensions;
        const char **wsi_instance_extensions = vd_fw_vk_wsi_instance_extensions(&num_wsi_instance_extensions);

        // Copy WSI Extensions
        for (int i = 0; i < num_wsi_instance_extensions; ++i) {
            all_instance_extensions[num_all_instance_extensions++] = wsi_instance_extensions[i];
        }

        // Set our own extensions
        for (int i = 0; i < num_required_instance_extensions; ++i) {
            all_instance_extensions[num_all_instance_extensions++] = required_instance_extensions[i];
        }

        // Check for instance extensions
        {
            uint32_t num_available_instance_extensions = 0;
            VK_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &num_available_instance_extensions, NULL));
            VkExtensionProperties *properties = ALLOC_ARRAY(VkExtensionProperties, num_available_instance_extensions);
            VK_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &num_available_instance_extensions, properties));

            for (int i = 0; i < num_all_instance_extensions; ++i) {

                int extension_is_available = 0;
                for (uint32_t j = 0; j < num_available_instance_extensions; ++j) {

                    if (strcmp(properties[j].extensionName, all_instance_extensions[i]) == 0) {
                        extension_is_available = 1;
                        break;
                    }
                }

                if (!extension_is_available) {
                    printf("Error: %s instance extension not found\n", all_instance_extensions[i]);
                    DBG_BREAK();
                }
            }

            free(properties);
        }

        // -------------------------------------------------------------------------------------------------------------
        // Instance Layers
        {
            uint32_t num_available_instance_layers = 0;
            VK_CHECK(vkEnumerateInstanceLayerProperties(&num_available_instance_layers, NULL));

            VkLayerProperties *layers = ALLOC_ARRAY(VkLayerProperties, num_available_instance_layers);
            VK_CHECK(vkEnumerateInstanceLayerProperties(&num_available_instance_layers, layers));

            for (int i = 0; i < num_required_instance_layers; ++i) {
                int layer_is_available = 0;
                for (uint32_t j = 0; j < num_available_instance_layers; ++j) {
                    if (strcmp(layers[j].layerName, required_instance_layers[i]) == 0) {
                        layer_is_available = 1;
                        break;
                    }
                }

                if (!layer_is_available) {
                    printf("Error: %s instance layer not found\n", required_instance_layers[i]);
                    DBG_BREAK();
                }
            }

            free(layers);
        }

        // -------------------------------------------------------------------------------------------------------------
        // Instance Creation
        VkApplicationInfo app_info;
        app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext              = 0;
        app_info.pApplicationName   = "vk_window";
        app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
        app_info.pEngineName        = "No Engine";
        app_info.engineVersion      = VK_MAKE_VERSION(1,0,0);
        app_info.apiVersion         = VK_API_VERSION_1_4;

        VkInstanceCreateFlags instance_create_flags = 0;
        if (vd_fw_get_platform() == VD_FW_PLATFORM_MACOS) {
            instance_create_flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        }

        VkInstanceCreateInfo instance_create_info;
        instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pNext                   = 0;
        instance_create_info.flags                   = instance_create_flags;
        instance_create_info.pApplicationInfo        = &app_info;
        instance_create_info.enabledLayerCount       = num_required_instance_layers;
        instance_create_info.ppEnabledLayerNames     = required_instance_layers;
        instance_create_info.enabledExtensionCount   = num_all_instance_extensions;
        instance_create_info.ppEnabledExtensionNames = all_instance_extensions;
        VK_CHECK(vkCreateInstance(&instance_create_info, NULL, &VK.instance));

        // -------------------------------------------------------------------------------------------------------------
        // Debug Messenger Creation
        VK.debug_utils_create_messenger = (PFN_vkCreateDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(VK.instance, "vkCreateDebugUtilsMessengerEXT");

        VK.debug_utils_destroy_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(VK.instance, "vkDestroyDebugUtilsMessengerEXT");

        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
        debug_messenger_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_messenger_create_info.pNext           = 0;
        debug_messenger_create_info.flags           = 0;
        debug_messenger_create_info.messageSeverity = 0
                                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                                      | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                                                      ;
        debug_messenger_create_info.messageType     = 0
                                                      | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                                      | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                                      | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                                                      // | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
                                                      ;
        debug_messenger_create_info.pfnUserCallback = debug_utils_messenger_callback;
        debug_messenger_create_info.pUserData       = 0;

        VK_CHECK(VK.debug_utils_create_messenger(VK.instance, &debug_messenger_create_info, NULL,
                                                 &VK.debug_utils_messenger));

        // -------------------------------------------------------------------------------------------------------------
        // Presentation Surface Creation
        vd_fw_vk_set_get_instance_proc_addr(vkGetInstanceProcAddr);
        VK_CHECK(vd_fw_vk_wsi_surface_create((void*)VK.instance, NULL, &VK.surface));

        // -------------------------------------------------------------------------------------------------------------
        // Physical Device Selection
        uint32_t num_physical_devices;
        VK_CHECK(vkEnumeratePhysicalDevices(VK.instance, &num_physical_devices, NULL));
        VkPhysicalDevice *physical_devices = ALLOC_ARRAY(VkPhysicalDevice, num_physical_devices);
        VK_CHECK(vkEnumeratePhysicalDevices(VK.instance, &num_physical_devices, physical_devices));

        int c_rank                          = -10;                            // Rank of best device so far
        VkPhysicalDevice c_preferred_device = VK_NULL_HANDLE;                 // Handle of best device
        uint32_t cg_q_family_index          = 0;                              // Queue family that supports graphics
        uint32_t cp_q_family_index          = 0;                              // Queue family that supports graphics
        VkSurfaceFormatKHR c_surface_format;                                  // Best surface format of best device
        uint32_t c_swapchain_image_count    = 0;                              // Min Image Count + 1 of best device
        VkPresentModeKHR c_present_mode     = VK_PRESENT_MODE_MAX_ENUM_KHR;   // Presentation mode
        c_surface_format.format = VK_FORMAT_UNDEFINED;
        c_surface_format.colorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;

        for (uint32_t i = 0; i < num_physical_devices; ++i) {
            int q_rank = 0;                       // The rank of the queried device (i)
            int q_can_use = 1;                    // Whether we can actually use this device
            uint32_t qg_q_family_index;           // Queue family of the queried device that supports graphics
            uint32_t qp_q_family_index;           // Queue family of the queried device that supports presentation
            VkSurfaceFormatKHR q_surface_format;  // Best surface format of queried device
            uint32_t q_swapchain_image_count = 0; // Min image count of surface caps of the queried device
            VkPresentModeKHR q_present_mode;      // Presentation mode of queried_device

            VkPhysicalDevice q_physical_device = physical_devices[i];
            q_surface_format.format = VK_FORMAT_UNDEFINED;
            q_surface_format.colorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR;
            q_present_mode = VK_PRESENT_MODE_MAX_ENUM_KHR;

            // ---------------------------------------------------------------------------------------------------------
            // Physical Device Properties
            VkPhysicalDeviceProperties q_properties;
            vkGetPhysicalDeviceProperties(q_physical_device, &q_properties);

            // Ensure the device supports the API version we want to use
            if (q_properties.apiVersion < api_version) {
                q_can_use = 0;
            }

            // Prefer discrete GPUs over everything else
            if (q_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                q_rank += 10;
            }

            // ---------------------------------------------------------------------------------------------------------
            // Physical Device Features
            VkPhysicalDeviceExtendedDynamicStateFeaturesEXT ext_dynamic_state_features;
            ext_dynamic_state_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
            ext_dynamic_state_features.pNext = 0;

            VkPhysicalDeviceVulkan13Features physical_device_vk13_features;
            physical_device_vk13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
            physical_device_vk13_features.pNext = &ext_dynamic_state_features;

            VkPhysicalDeviceVulkan12Features physical_device_vk12_features;
            physical_device_vk12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
            physical_device_vk12_features.pNext = &physical_device_vk13_features;

            VkPhysicalDeviceVulkan11Features physical_device_vk11_features;
            physical_device_vk11_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
            physical_device_vk11_features.pNext = &physical_device_vk12_features;

            VkPhysicalDeviceFeatures2 physical_device_features2;
            physical_device_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
            physical_device_features2.pNext = &physical_device_vk11_features;
            vkGetPhysicalDeviceFeatures2(q_physical_device, &physical_device_features2);

            if (q_can_use) {
                int features_are_available = 1
                                             && physical_device_vk11_features.shaderDrawParameters
                                             && physical_device_vk13_features.dynamicRendering
                                             && physical_device_vk13_features.synchronization2
                                             && ext_dynamic_state_features.extendedDynamicState
                                             ;
                if (!features_are_available) {
                    q_can_use = 0;
                }
            }

            // ---------------------------------------------------------------------------------------------------------
            // Device Extension Support
            uint32_t num_available_device_extensions;
            VK_CHECK(vkEnumerateDeviceExtensionProperties(q_physical_device, NULL, 
                                                          &num_available_device_extensions, NULL));

            VkExtensionProperties *available_device_extensions = ALLOC_ARRAY(VkExtensionProperties,
                                                                             num_available_device_extensions);

            VK_CHECK(vkEnumerateDeviceExtensionProperties(q_physical_device, NULL, 
                                                          &num_available_device_extensions,
                                                          available_device_extensions));

            for (int x = 0; x < num_required_device_extensions; ++x) {
                int device_extension_found = 0;
                for (uint32_t j = 0; j < num_available_device_extensions; ++j) {
                    if (strcmp(available_device_extensions[j].extensionName, required_device_extensions[x]) == 0) {
                        device_extension_found = 1;
                        break;
                    }
                }

                if (!device_extension_found) {
                    q_can_use = 0;
                    break;
                }
            }

            free(available_device_extensions);

            // ---------------------------------------------------------------------------------------------------------
            // Queue Family Enumeration
            uint32_t q_num_families;
            vkGetPhysicalDeviceQueueFamilyProperties(q_physical_device, &q_num_families, NULL);

            VkQueueFamilyProperties *q_families = ALLOC_ARRAY(VkQueueFamilyProperties, q_num_families);
            vkGetPhysicalDeviceQueueFamilyProperties(q_physical_device, &q_num_families, q_families);

            qg_q_family_index = q_num_families;
            qp_q_family_index = q_num_families;

            for (uint32_t q = 0; q < q_num_families; ++q) {
                VkQueueFamilyProperties *q_family_properties = &q_families[q];

                VkBool32 surface_support;
                VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(q_physical_device, q, VK.surface, &surface_support));

                // If graphics queue family index not assigned yet, check if this queue family is ok
                if (qg_q_family_index == q_num_families) {

                    // At least 1 queue family of any queue count that supports the following bits
                    int can_use_queue_family = 1
                                               && (q_family_properties->queueFlags & VK_QUEUE_GRAPHICS_BIT)
                                               && (q_family_properties->queueFlags & VK_QUEUE_TRANSFER_BIT)
                                               ;

                    if (can_use_queue_family) {
                        qg_q_family_index = q;
                    }
                }

                if (qp_q_family_index == q_num_families) {
                    // At least 1 queue family of any queue count that supports presentation
                    int can_use_queue_family = 1
                                               && surface_support
                                               ;

                    if (can_use_queue_family) {
                        qp_q_family_index = q;
                    }
                }

                // If both queue families assigned, stop.
                if ((qg_q_family_index != q_num_families) && (qp_q_family_index != q_num_families)) {
                    break;
                }

            }

            // If no queues can support us, don't even bother ranking the device
            if ((qg_q_family_index == q_num_families) || (qp_q_family_index == q_num_families)) {
                q_can_use = 0;
            }

            // ---------------------------------------------------------------------------------------------------------
            // SwapChain Surface & Presentation Capabilities Support
            if (q_can_use) {
                VkSurfaceCapabilitiesKHR surface_caps;
                VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(q_physical_device, VK.surface, &surface_caps));
                q_swapchain_image_count = surface_caps.minImageCount + 1;
                if (q_swapchain_image_count > surface_caps.maxImageCount) {
                    q_swapchain_image_count = surface_caps.maxImageCount;
                }

                // Surface Formats
                uint32_t num_surface_formats;
                VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(q_physical_device, VK.surface,
                                                              &num_surface_formats, NULL));

                VkSurfaceFormatKHR *surface_formats = ALLOC_ARRAY(VkSurfaceFormatKHR, num_surface_formats);
                VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(q_physical_device, VK.surface,
                                                              &num_surface_formats, surface_formats));

                for (uint32_t f = 0; f < num_surface_formats; ++f) {
                    if (q_surface_format.format == VK_FORMAT_UNDEFINED) {
                        q_surface_format = surface_formats[f];
                    }

                    if ((surface_formats[f].format == VK_FORMAT_B8G8R8A8_SRGB)
                        && (surface_formats[f].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
                    {
                        q_surface_format = surface_formats[f];
                        break;
                    }
                }

                free(surface_formats);

                // If for some reason it has no surface formats, then we can't use it
                if (q_surface_format.format == VK_FORMAT_UNDEFINED) {
                    q_can_use = 0;
                }

                // Presentation Modes
                uint32_t num_present_modes;
                VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(q_physical_device, VK.surface,
                                                                   &num_present_modes, NULL));
                VkPresentModeKHR *present_modes = ALLOC_ARRAY(VkPresentModeKHR, num_present_modes);
                VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(q_physical_device, VK.surface,
                                                                   &num_present_modes, present_modes));

                for (uint32_t p = 0; p < num_present_modes; ++p) {
                    if (q_present_mode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
                        q_present_mode = present_modes[p];
                    }

                    if (present_modes[p] == VK_PRESENT_MODE_MAILBOX_KHR) {
                        q_present_mode = present_modes[p];
                    }
                }

                if (q_present_mode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
                    q_can_use = 0;
                }
            }

            if (q_can_use) {
                q_rank += 1;
            }

            if (q_can_use) {
                if (c_rank < q_rank) {
                    c_rank                  = q_rank;
                    c_preferred_device      = physical_devices[i];
                    cg_q_family_index       = qg_q_family_index;
                    cp_q_family_index       = qp_q_family_index;
                    c_surface_format        = q_surface_format;
                    c_swapchain_image_count = q_swapchain_image_count;
                    c_present_mode          = q_present_mode;
                }
            }

            free(q_families);
        }

        if (c_preferred_device == VK_NULL_HANDLE) {
            printf("Error. Failed to find suitable physical device\n");
            DBG_BREAK();
        }

        free(physical_devices);
        VK.min_swapchain_image_count = c_swapchain_image_count;
        VK.present_mode = c_present_mode;

        // -------------------------------------------------------------------------------------------------------------
        // Logical Device Creation
        VkDeviceQueueCreateInfo device_queue_create_infos[2];
        float                   device_queue_priorities[1] = { 1.f };
        int                     num_device_queue_create_infos = 1;

        device_queue_create_infos[0].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_infos[0].pNext            = 0;
        device_queue_create_infos[0].flags            = 0;
        device_queue_create_infos[0].queueFamilyIndex = cg_q_family_index;
        device_queue_create_infos[0].queueCount       = 1;
        device_queue_create_infos[0].pQueuePriorities = device_queue_priorities;

        // If the graphics & present queue families are not actually the same
        if (cg_q_family_index != cp_q_family_index) {
            device_queue_create_infos[1].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            device_queue_create_infos[1].pNext            = 0;
            device_queue_create_infos[1].flags            = 0;
            device_queue_create_infos[1].queueFamilyIndex = cp_q_family_index;
            device_queue_create_infos[1].queueCount       = 1;
            device_queue_create_infos[1].pQueuePriorities = device_queue_priorities;
            num_device_queue_create_infos++;
        }

        VkPhysicalDeviceSwapchainMaintenance1FeaturesKHR physical_device_swapchain_maintenance1_features;
        physical_device_swapchain_maintenance1_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_KHR;
        physical_device_swapchain_maintenance1_features.pNext = 0;

        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT ext_dynamic_state_features;
        memset(&ext_dynamic_state_features, 0, sizeof(ext_dynamic_state_features));
        ext_dynamic_state_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
        ext_dynamic_state_features.pNext = &physical_device_swapchain_maintenance1_features;

        VkPhysicalDeviceVulkan13Features physical_device_vk13_features;
        memset(&physical_device_vk13_features, 0, sizeof(physical_device_vk13_features));
        physical_device_vk13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
        physical_device_vk13_features.pNext = &ext_dynamic_state_features;

        VkPhysicalDeviceVulkan12Features physical_device_vk12_features;
        memset(&physical_device_vk12_features, 0, sizeof(physical_device_vk12_features));
        physical_device_vk12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
        physical_device_vk12_features.pNext = &physical_device_vk13_features;

        VkPhysicalDeviceVulkan11Features physical_device_vk11_features;
        memset(&physical_device_vk11_features, 0, sizeof(physical_device_vk11_features));
        physical_device_vk11_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
        physical_device_vk11_features.pNext = &physical_device_vk12_features;

        VkPhysicalDeviceFeatures2 physical_device_features2;
        memset(&physical_device_features2, 0, sizeof(physical_device_features2));
        physical_device_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        physical_device_features2.pNext = &physical_device_vk11_features;

        physical_device_vk11_features.shaderDrawParameters = VK_TRUE;
        physical_device_vk13_features.dynamicRendering = VK_TRUE;
        physical_device_vk13_features.synchronization2 = VK_TRUE;
        ext_dynamic_state_features.extendedDynamicState = VK_TRUE;
        physical_device_swapchain_maintenance1_features.swapchainMaintenance1 = VK_TRUE;

        VkDeviceCreateInfo device_create_info;
        device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pNext                   = &physical_device_features2;
        device_create_info.flags                   = 0;
        device_create_info.queueCreateInfoCount    = num_device_queue_create_infos;
        device_create_info.pQueueCreateInfos       = device_queue_create_infos;
        device_create_info.enabledLayerCount       = 0; // Deprecated
        device_create_info.ppEnabledLayerNames     = 0; // Deprecated
        device_create_info.enabledExtensionCount   = num_required_device_extensions;
        device_create_info.ppEnabledExtensionNames = required_device_extensions;
        device_create_info.pEnabledFeatures        = 0;

        VK.physical_device = c_preferred_device;
        VK_CHECK(vkCreateDevice(c_preferred_device, &device_create_info, NULL, &VK.device));

        VK.graphics_queue_family = cg_q_family_index;
        vkGetDeviceQueue(VK.device, cg_q_family_index, 0, &VK.graphics_queue);

        VK.present_queue_family = cp_q_family_index;
        vkGetDeviceQueue(VK.device, cp_q_family_index, 0, &VK.present_queue);

        VK.surface_format = c_surface_format;

        // -------------------------------------------------------------------------------------------------------------
        // Swapchain Creation

        create_swapchain_and_image_views(window_size);

        // -------------------------------------------------------------------------------------------------------------
        // Command Pool Creation
        VkCommandPoolCreateInfo command_pool_create_info;
        command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.pNext            = 0;
        command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        command_pool_create_info.queueFamilyIndex = cg_q_family_index;
        VK_CHECK(vkCreateCommandPool(VK.device, &command_pool_create_info, NULL, &VK.cmd_pool));

        // -------------------------------------------------------------------------------------------------------------
        // In-Flight Frame Data
        VK.num_frames = frames_in_flight;
        VK.frames = ALLOC_ARRAY(VkFrameData, VK.num_frames);
        for (uint32_t i = 0; i < VK.num_frames; ++i) {
            VkFrameData *frame = &VK.frames[i];
            VkCommandBufferAllocateInfo cmd_buffer_allocate_info;
            cmd_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            cmd_buffer_allocate_info.pNext              = 0;
            cmd_buffer_allocate_info.commandPool        = VK.cmd_pool;
            cmd_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            cmd_buffer_allocate_info.commandBufferCount = 1;
            VK_CHECK(vkAllocateCommandBuffers(VK.device, &cmd_buffer_allocate_info, &frame->cmd_buffer));

            VkSemaphoreCreateInfo semaphore_create_info;
            semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            semaphore_create_info.pNext = 0;
            semaphore_create_info.flags = 0;
            VK_CHECK(vkCreateSemaphore(VK.device, &semaphore_create_info, NULL, &frame->sem_present_complete));

            VkFenceCreateInfo fence_create_info;
            fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fence_create_info.pNext = 0;
            fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            VK_CHECK(vkCreateFence(VK.device, &fence_create_info, NULL, &frame->fnc_render_complete));
        }

        // -------------------------------------------------------------------------------------------------------------
        // Graphics Pipeline
        VkPipelineLayoutCreateInfo pipeline_layout_create_info;
        pipeline_layout_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.pNext                  = 0;
        pipeline_layout_create_info.flags                  = 0;
        pipeline_layout_create_info.setLayoutCount         = 0;
        pipeline_layout_create_info.pSetLayouts            = 0;
        pipeline_layout_create_info.pushConstantRangeCount = 0;
        pipeline_layout_create_info.pPushConstantRanges    = 0;
        VK_CHECK(vkCreatePipelineLayout(VK.device, &pipeline_layout_create_info, NULL, &VK.pipeline_layout));

        size_t sz; 
        void *spv = spriv_load("_spirv/vk_shader_basic.spirv", &sz);

        VkShaderModuleCreateInfo shader_module_create_info;
        shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.pNext    = 0;
        shader_module_create_info.flags    = 0;
        shader_module_create_info.codeSize = sz;
        shader_module_create_info.pCode    = spv;
        VK_CHECK(vkCreateShaderModule(VK.device, &shader_module_create_info, NULL, &VK.shader_module));

        VkPipelineShaderStageCreateInfo vert_shader_stage_create_info;
        vert_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vert_shader_stage_create_info.pNext = 0;
        vert_shader_stage_create_info.flags = 0;
        vert_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_shader_stage_create_info.module = VK.shader_module;
        vert_shader_stage_create_info.pName = "vert_main";
        vert_shader_stage_create_info.pSpecializationInfo = 0;

        VkPipelineShaderStageCreateInfo frag_shader_stage_create_info;
        frag_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        frag_shader_stage_create_info.pNext = 0;
        frag_shader_stage_create_info.flags = 0;
        frag_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_shader_stage_create_info.module = VK.shader_module;
        frag_shader_stage_create_info.pName = "frag_main";
        frag_shader_stage_create_info.pSpecializationInfo = 0;

        VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_create_info, frag_shader_stage_create_info };

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
        vertex_input_state_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.pNext                           = 0;
        vertex_input_state_create_info.flags                           = 0;
        vertex_input_state_create_info.vertexBindingDescriptionCount   = 0;
        vertex_input_state_create_info.pVertexBindingDescriptions      = 0;
        vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
        vertex_input_state_create_info.pVertexAttributeDescriptions    = 0;

        VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info;
        pipeline_input_assembly_state_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipeline_input_assembly_state_create_info.pNext                  = 0;
        pipeline_input_assembly_state_create_info.flags                  = 0;
        pipeline_input_assembly_state_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info;
        pipeline_viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipeline_viewport_state_create_info.pNext         = 0;
        pipeline_viewport_state_create_info.flags         = 0;
        pipeline_viewport_state_create_info.viewportCount = 1;
        pipeline_viewport_state_create_info.pViewports    = 0;
        pipeline_viewport_state_create_info.scissorCount  = 1;
        pipeline_viewport_state_create_info.pScissors     = 0;

        VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info;
        pipeline_rasterization_state_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipeline_rasterization_state_create_info.pNext                   = 0;
        pipeline_rasterization_state_create_info.flags                   = 0;
        pipeline_rasterization_state_create_info.depthClampEnable        = VK_FALSE;
        pipeline_rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        pipeline_rasterization_state_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
        pipeline_rasterization_state_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
        pipeline_rasterization_state_create_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
        pipeline_rasterization_state_create_info.depthBiasEnable         = VK_FALSE;
        pipeline_rasterization_state_create_info.depthBiasConstantFactor = 0.f;
        pipeline_rasterization_state_create_info.depthBiasClamp          = 0.f;
        pipeline_rasterization_state_create_info.depthBiasSlopeFactor    = 1.f;
        pipeline_rasterization_state_create_info.lineWidth               = 1.f;

        VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info;
        pipeline_multisample_state_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipeline_multisample_state_create_info.pNext                 = 0;
        pipeline_multisample_state_create_info.flags                 = 0;
        pipeline_multisample_state_create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
        pipeline_multisample_state_create_info.sampleShadingEnable   = VK_FALSE;
        pipeline_multisample_state_create_info.minSampleShading      = 0.f;
        pipeline_multisample_state_create_info.pSampleMask           = 0;
        pipeline_multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
        pipeline_multisample_state_create_info.alphaToOneEnable      = VK_FALSE;

        VkPipelineColorBlendAttachmentState color_blend_attachment_state;
        color_blend_attachment_state.blendEnable         = VK_FALSE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.colorBlendOp        = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp        = VK_BLEND_OP_ADD;
        color_blend_attachment_state.colorWriteMask      = 0
                                                           | VK_COLOR_COMPONENT_R_BIT
                                                           | VK_COLOR_COMPONENT_G_BIT 
                                                           | VK_COLOR_COMPONENT_B_BIT 
                                                           | VK_COLOR_COMPONENT_A_BIT 
                                                           ;

        VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info;
        pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipeline_color_blend_state_create_info.pNext = 0;
        pipeline_color_blend_state_create_info.flags = 0;
        pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
        pipeline_color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
        pipeline_color_blend_state_create_info.attachmentCount = 1;
        pipeline_color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
        pipeline_color_blend_state_create_info.blendConstants[0] = 0.f;
        pipeline_color_blend_state_create_info.blendConstants[1] = 0.f;
        pipeline_color_blend_state_create_info.blendConstants[2] = 0.f;
        pipeline_color_blend_state_create_info.blendConstants[3] = 0.f;

        VkDynamicState dynamic_states[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };

        VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info;
        pipeline_dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipeline_dynamic_state_create_info.pNext             = 0;
        pipeline_dynamic_state_create_info.flags             = 0;
        pipeline_dynamic_state_create_info.dynamicStateCount = sizeof(dynamic_states) / sizeof(dynamic_states[0]);
        pipeline_dynamic_state_create_info.pDynamicStates    = dynamic_states;

        VkPipelineRenderingCreateInfo pipeline_rendering_create_info;
        pipeline_rendering_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        pipeline_rendering_create_info.pNext                   = 0;
        pipeline_rendering_create_info.viewMask                = 0;
        pipeline_rendering_create_info.colorAttachmentCount    = 1;
        pipeline_rendering_create_info.pColorAttachmentFormats = &c_surface_format.format;
        pipeline_rendering_create_info.depthAttachmentFormat   = VK_FORMAT_UNDEFINED;
        pipeline_rendering_create_info.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

        VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
        graphics_pipeline_create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_create_info.pNext               = &pipeline_rendering_create_info;
        graphics_pipeline_create_info.flags               = 0;
        graphics_pipeline_create_info.stageCount          = 2;
        graphics_pipeline_create_info.pStages             = shader_stages;
        graphics_pipeline_create_info.pVertexInputState   = &vertex_input_state_create_info;
        graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
        graphics_pipeline_create_info.pTessellationState  = 0;
        graphics_pipeline_create_info.pViewportState      = &pipeline_viewport_state_create_info;
        graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
        graphics_pipeline_create_info.pMultisampleState   = &pipeline_multisample_state_create_info;
        graphics_pipeline_create_info.pDepthStencilState  = 0;
        graphics_pipeline_create_info.pColorBlendState    = &pipeline_color_blend_state_create_info;
        graphics_pipeline_create_info.pDynamicState       = &pipeline_dynamic_state_create_info;
        graphics_pipeline_create_info.layout              = VK.pipeline_layout;
        graphics_pipeline_create_info.renderPass          = VK_NULL_HANDLE;
        graphics_pipeline_create_info.subpass             = 0;
        graphics_pipeline_create_info.basePipelineHandle  = VK_NULL_HANDLE;
        graphics_pipeline_create_info.basePipelineIndex   = 0;

        VK_CHECK(vkCreateGraphicsPipelines(VK.device, VK_NULL_HANDLE,
                                           1, &graphics_pipeline_create_info,
                                           NULL, &VK.graphics_pipeline));
    }

    while (vd_fw_running()) {

        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();
        int new_w, new_h;
        if (vd_fw_get_size(&new_w, &new_h)) {
            window_size.width = (uint32_t)new_w;
            window_size.height = (uint32_t)new_h;
            // Size changed. Recreate swapchain & image views
            window_size = create_swapchain_and_image_views();
        } 

        VkFrameData *frame = &VK.frames[VK.curr_frame];
        VK_CHECK(vkWaitForFences(VK.device, 1, &frame->fnc_render_complete, VK_TRUE, UINT64_MAX));
        vkResetFences(VK.device, 1, &frame->fnc_render_complete);

        uint32_t swapchain_image_idx;
        VkResult acquire_result = vkAcquireNextImageKHR(VK.device, VK.swapchain,
                                                        UINT64_MAX, frame->sem_present_complete,
                                                        VK_NULL_HANDLE, &swapchain_image_idx);

        if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR) {
            VK_CHECK(acquire_result);
        }

        VkSemaphore sem_render_complete = VK.sems_render_complete[swapchain_image_idx];

        VkCommandBuffer cmd = frame->cmd_buffer;
        vkResetCommandBuffer(cmd, 0);

        VkCommandBufferBeginInfo cmd_begin_info;
        cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmd_begin_info.pNext = 0;
        cmd_begin_info.flags = 0;
        cmd_begin_info.pInheritanceInfo = 0;
        VK_CHECK(vkBeginCommandBuffer(cmd, &cmd_begin_info));
        {
            swapchain_image_barrier(cmd, swapchain_image_idx,
                                    VK.graphics_queue_family,                       VK.graphics_queue_family,
                                    VK_IMAGE_LAYOUT_UNDEFINED,                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                    0,                                              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

            VkRenderingAttachmentInfo color_attachment_info;
            color_attachment_info.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
            color_attachment_info.pNext              = 0;
            color_attachment_info.imageView          = VK.swapchain_image_views[swapchain_image_idx];
            color_attachment_info.imageLayout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            color_attachment_info.resolveMode        = VK_RESOLVE_MODE_NONE;
            color_attachment_info.resolveImageView   = VK_NULL_HANDLE;
            color_attachment_info.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            color_attachment_info.loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attachment_info.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
            color_attachment_info.clearValue.color   = clear_color_value;

            VkRenderingInfo rendering_info;
            rendering_info.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
            rendering_info.pNext                = 0;
            rendering_info.flags                = 0;
            rendering_info.renderArea.offset    = (VkOffset2D){0, 0};
            rendering_info.renderArea.extent    = window_size;
            rendering_info.layerCount           = 1;
            rendering_info.viewMask             = 0;
            rendering_info.colorAttachmentCount = 1;
            rendering_info.pColorAttachments    = &color_attachment_info;
            rendering_info.pDepthAttachment     = 0;
            rendering_info.pStencilAttachment   = 0;
            vkCmdBeginRendering(cmd, &rendering_info);

            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, VK.graphics_pipeline);

            VkViewport viewport = {
                0.f, 0.f,
                (float)window_size.width, (float)window_size.height,
                0.f, 1.f
            };
            vkCmdSetViewport(cmd, 0, 1, &viewport);

            VkRect2D scissor = {
                {0, 0},
                window_size,
            };
            vkCmdSetScissor(cmd, 0, 1, &scissor);

            vkCmdDraw(cmd, 3, 1, 0, 0);

            vkCmdEndRendering(cmd);

            swapchain_image_barrier(cmd, swapchain_image_idx,
                                    VK.graphics_queue_family,                       VK.graphics_queue_family,
                                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           0,
                                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

        }
        VK_CHECK(vkEndCommandBuffer(cmd));

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submit_info;
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = 0;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &frame->sem_present_complete;
        submit_info.pWaitDstStageMask = &wait_stage;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmd;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &sem_render_complete;

        VK_CHECK(vkQueueSubmit(VK.graphics_queue, 1, &submit_info, frame->fnc_render_complete));

        VkPresentInfoKHR present_info;
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pNext = 0;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &sem_render_complete;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &VK.swapchain;
        present_info.pImageIndices = &swapchain_image_idx;
        present_info.pResults = 0;
        VkResult present_result = vkQueuePresentKHR(VK.present_queue, &present_info);
        if (1
            && (present_result != VK_ERROR_OUT_OF_DATE_KHR)
            && (present_result != VK_SUBOPTIMAL_KHR)
            && (present_result != VK_SUCCESS)
            )
        {
            VK_CHECK(present_result);
        }

        vd_fw_unlock();

        VK.curr_frame++;
        if (VK.curr_frame >= VK.num_frames) {
            VK.curr_frame = 0;
        }
    }

    VK_CHECK(vkDeviceWaitIdle(VK.device));

    for (uint32_t i = 0; i < VK.num_frames; ++i) {
        VkFrameData *frame = &VK.frames[i];
        vkDestroySemaphore(VK.device, frame->sem_present_complete, NULL);
        vkDestroyFence(VK.device, frame->fnc_render_complete, NULL);
    }

    free(VK.frames);

    vkDestroyCommandPool(VK.device, VK.cmd_pool, NULL);
    vkDestroyShaderModule(VK.device, VK.shader_module, NULL);
    vkDestroyPipeline(VK.device, VK.graphics_pipeline, NULL);
    vkDestroyPipelineLayout(VK.device, VK.pipeline_layout, NULL);

    for (uint32_t i = 0; i < VK.num_swapchain_images; ++i) {
        vkDestroyImageView(VK.device, VK.swapchain_image_views[i], NULL);
        vkDestroySemaphore(VK.device, VK.sems_render_complete[i], NULL);
    }

    free(VK.swapchain_images);
    free(VK.swapchain_image_views);
    free(VK.sems_render_complete);

    vkDestroySwapchainKHR(VK.device, VK.swapchain, NULL);

    vkDestroyDevice(VK.device, NULL);
    vkDestroySurfaceKHR(VK.instance, VK.surface, NULL);
    VK.debug_utils_destroy_messenger(VK.instance, VK.debug_utils_messenger, NULL);
    vkDestroyInstance(VK.instance, NULL);
    return 0;
}

static VkBool32 debug_utils_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                               VkDebugUtilsMessageTypeFlagsEXT message_types,
                                               const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                               void *usr_data)
{
    (void)message_types;
    (void)usr_data;
    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        printf("[Vulkan/Verbose]:  %s\n", callback_data->pMessage);
    }

    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        printf("[Vulkan/Info   ]:   %s\n", callback_data->pMessage);
    }

    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        printf("[Vulkan/Warning]:   %s\n", callback_data->pMessage);
    }

    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        printf("[Vulkan/Error  ]:  %s\n", callback_data->pMessage);
    }

    return VK_TRUE;
}

static void swapchain_image_barrier(VkCommandBuffer cmd, uint32_t image_index,
                                    uint32_t src_queue_family, uint32_t dst_queue_family,
                                    VkImageLayout old_layout, VkImageLayout new_layout,
                                    VkAccessFlags2 src_access, VkAccessFlags2 dst_access,
                                    VkPipelineStageFlags2 src_stage, VkPipelineStageFlags2 dst_stage)
{
    VkImageMemoryBarrier2 barrier;
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.pNext = 0;
    barrier.srcStageMask = src_stage;
    barrier.srcAccessMask = src_access;
    barrier.dstStageMask = dst_stage;
    barrier.dstAccessMask = dst_access;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = src_queue_family;
    barrier.dstQueueFamilyIndex = dst_queue_family;
    barrier.image = VK.swapchain_images[image_index];
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkDependencyInfo dependency_info = {0};
    dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.imageMemoryBarrierCount = 1;
    dependency_info.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(cmd, &dependency_info);
}

static VkExtent2D create_swapchain_and_image_views()
{
    VK_CHECK(vkDeviceWaitIdle(VK.device));

    VkSurfaceCapabilitiesKHR surface_caps;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VK.physical_device, VK.surface, &surface_caps));
    VkExtent2D size = surface_caps.maxImageExtent;

    VkSwapchainKHR old_swapchain = VK.swapchain;
    if (VK.swapchain) {
        // Free swapchain data

        for (uint32_t i = 0; i < VK.num_swapchain_images; ++i) {
            vkDestroyImageView(VK.device, VK.swapchain_image_views[i], NULL);
            vkDestroySemaphore(VK.device, VK.sems_render_complete[i], NULL);
        }

        free(VK.swapchain_images);
        free(VK.swapchain_image_views);
        free(VK.sems_render_complete);

    }

    uint32_t swapchain_queues[2] = {VK.present_queue_family, VK.graphics_queue_family};
    uint32_t num_swapchain_queues = 1;
    VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;

    if (VK.graphics_queue_family != VK.present_queue_family) {
        // If the graphics & present queue families are not actually the same
        // Then we need to use it concurrently, and also add access to the graphics queue, since we're
        // drawing into it.
        swapchain_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
        num_swapchain_queues++;
    }

    VkSwapchainPresentScalingCreateInfoKHR swapchain_present_scaling_create_info;
    swapchain_present_scaling_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_SCALING_CREATE_INFO_KHR;
    swapchain_present_scaling_create_info.pNext = 0;
    swapchain_present_scaling_create_info.scalingBehavior = VK_PRESENT_SCALING_STRETCH_BIT_KHR;
    // swapchain_present_scaling_create_info.scalingBehavior = VK_PRESENT_SCALING_ONE_TO_ONE_BIT_KHR;
    swapchain_present_scaling_create_info.presentGravityX = VK_PRESENT_GRAVITY_MIN_BIT_KHR;
    swapchain_present_scaling_create_info.presentGravityY = VK_PRESENT_GRAVITY_MIN_BIT_KHR;

    VkSwapchainCreateInfoKHR swapchain_create_info;
    swapchain_create_info.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext                 = &swapchain_present_scaling_create_info;
    swapchain_create_info.flags                 = 0;
    swapchain_create_info.surface               = VK.surface;
    swapchain_create_info.minImageCount         = VK.min_swapchain_image_count;
    swapchain_create_info.imageFormat           = VK.surface_format.format;
    swapchain_create_info.imageColorSpace       = VK.surface_format.colorSpace;
    swapchain_create_info.imageExtent           = size;
    swapchain_create_info.imageArrayLayers      = 1;
    swapchain_create_info.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = num_swapchain_queues;
    swapchain_create_info.pQueueFamilyIndices   = swapchain_queues;
    swapchain_create_info.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchain_create_info.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode           = VK.present_mode;
    swapchain_create_info.clipped               = VK_TRUE;
    swapchain_create_info.oldSwapchain          = old_swapchain;

    VK_CHECK(vkCreateSwapchainKHR(VK.device, &swapchain_create_info, NULL, &VK.swapchain));

    VK_CHECK(vkGetSwapchainImagesKHR(VK.device, VK.swapchain, &VK.num_swapchain_images, 0));
    VK.swapchain_images = ALLOC_ARRAY(VkImage, VK.num_swapchain_images);
    VK_CHECK(vkGetSwapchainImagesKHR(VK.device, VK.swapchain, &VK.num_swapchain_images, VK.swapchain_images));

    VK.swapchain_image_views = ALLOC_ARRAY(VkImageView, VK.num_swapchain_images);
    VK.sems_render_complete = ALLOC_ARRAY(VkSemaphore, VK.num_swapchain_images);
    for (uint32_t i = 0; i < VK.num_swapchain_images; ++i) {
        VkImageViewCreateInfo image_view_create_info;
        image_view_create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext                           = 0;
        image_view_create_info.flags                           = 0;
        image_view_create_info.image                           = VK.swapchain_images[i]; 
        image_view_create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format                          = VK.surface_format.format;
        image_view_create_info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel   = 0;
        image_view_create_info.subresourceRange.levelCount     = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount     = 1;
        VK_CHECK(vkCreateImageView(VK.device, &image_view_create_info, NULL, &VK.swapchain_image_views[i]));

        VkSemaphoreCreateInfo semaphore_create_info;
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphore_create_info.pNext = 0;
        semaphore_create_info.flags = 0;
        VK_CHECK(vkCreateSemaphore(VK.device, &semaphore_create_info, NULL, &VK.sems_render_complete[i]));
    }

    if (old_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(VK.device, old_swapchain, NULL);
    }

    return size;
}

static void *spriv_load(const char *path, size_t *size)
{
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    size_t alloc_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void *result = malloc(alloc_size);
    fread(result, alloc_size, 1, f);
    fclose(f);

    *size = alloc_size;
    return result;
}

#define VD_FW_IMPL
#include "vd_fw.h"
