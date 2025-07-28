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

#ifndef VD_VK_MACRO_ABBREVIATIONS
#define VD_VK_MACRO_ABBREVIATIONS VD_MACRO_ABBREVIATIONS
#endif // !VD_VK_MACRO_ABBREVIATIONS

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

typedef struct __VD_VK_GetPhysicalDevicePresentationSupportInfo {
    VkInstance          instance;
    VkPhysicalDevice    physical_device;
    VD(u32)             queue_family_index;
} VD(VkGetPhysicalDevicePresentationSupportInfo);

#define VD_VK_PROC_GET_PHYSICAL_DEVICE_PRESENTATION_SUPPORT(name) VD(b32) name(VD(VkGetPhysicalDevicePresentationSupportInfo) *info)
typedef VD_VK_PROC_GET_PHYSICAL_DEVICE_PRESENTATION_SUPPORT(VD(VkProcGetPhysicalDevicePresentationSupport));

typedef struct __VD_VK_CreateWSISurfaceInfo {
    VkInstance instance;
} VD(VkCreateWSISurfaceInfo);

#define VD_VK_PROC_CREATE_WSI_SURFACE(name) VkResult name(VD(VkCreateWSISurfaceInfo) *info, VkSurfaceKHR *surface)
typedef VD_VK_PROC_CREATE_WSI_SURFACE(VD(VkProcCreateWSISurface));

typedef struct __VD_VK_RequireInstanceExtensionsInfo {
    VD(usize)       count;
    const VD(cstr)  *extensions;
    VD(b32)         log;
} VD(VkRequireInstanceExtensionsInfo);

typedef struct __VD_VK_RequireInstanceLayersInfo {
    VD(usize)       count;
    const VD(cstr)  *layers;
} VD(VkRequireInstanceLayersInfo);

typedef struct __VD_VK_CreateInstanceExtendedInfo {
    const VD(cstr)                              app_name;
    VD(u32)                                     app_version;
    const VD(cstr)                              engine_name;
    VD(u32)                                     engine_version; 
    VD(u32)                                     api_version;

    VD(usize)                                   num_instance_extensions;
    const VD(cstr)                              *instance_extensions;

    VD(usize)                                   num_instance_layers;
    const VD(cstr)                              *instance_layers;

    VkFlags                                     flags;

    /** Enables debug utils. For this to work "VK_EXT_debug_utils" must in @instance_extensions */
    struct {
        VD(b32)                                 on;
        PFN_vkDebugUtilsMessengerCallbackEXT    messenger_callback;
    } debug_utils;

    struct {
        VkInstance                  *instance;
        VkDebugUtilsMessengerEXT    *debug_messenger;
    } result;
} VD(VkCreateInstanceExtendedInfo);

typedef enum {
    VD_(VK_QUEUE_CAPABILITIES_GRAPHICS) = 1 << 0,
    VD_(VK_QUEUE_CAPABILITIES_COMPUTE)  = 1 << 1,
    VD_(VK_QUEUE_CAPABILITIES_TRANSFER) = 1 << 2,
    VD_(VK_QUEUE_CAPABILITIES_PRESENT)  = 1 << 3,
} VD(VkQueueCapabilities);

/** @doc VkQueueSetup
 * Describes a queue setup that is applied when:
 * - Checking if a physical device supports it
 * - Creating the logical device
 */
typedef struct __VD_VK_QueueSetup {
    /** The capability bits of the queue family required (or present) */
    VD(VkQueueCapabilities)     capabilities;
    /** The amount of queues required (or present) */
    VD(u32)                     required_count;
    /** The amount of queues present */
    VD(u32)                     present_count;
    /** The queue family index that matches these requirements */
    VD(i32)                     cached_index;
} VD(VkQueueSetup);

typedef struct __VD_VK_PhysicalDeviceCharacteristics {
    VD(b32)             multi_draw_indirect;
    VD(b32)             buffer_device_addr;
    VD(b32)             descriptor_indexing;
    VD(b32)             synchronization2;
    VD(b32)             dynamic_rendering;
    /** Multiplies rank by 2 to indicate preference */
    VD(b32)             prefer_discrete;
    VD(b32)             prefer_integrated;

    VD(usize)           num_extensions;
    const VD(cstr)      *extensions;

    VD(usize)           num_queue_setups;
    VD(VkQueueSetup)    *queue_setups;

    VD(i32)             rank_baseline;
    /** Internal use only; Used to store rank of 
     * @VkPickPhysicalDeviceInfo.compatible_characteristics
     * computed_rank <= 0: physical device with characteristics is not available
     * computed_rank  > 0: physical device is available; higher rank is more preferential
     */
    VD(i32)             computed_rank;
    VkPhysicalDevice    physical_device;
} VD(VkPhysicalDeviceCharacteristics);

typedef struct __VD_VK_PickPhysicalDeviceInfo {
    VkInstance                                      instance;
    VD(usize)                                       num_compatible_characteristics;
    VD(VkPhysicalDeviceCharacteristics)             *compatible_characteristics;
    VD(VkProcGetPhysicalDevicePresentationSupport)  *get_physical_device_presentation_support;

    struct {
        VkPhysicalDevice                            *physical_device;
        VkDevice                                    *device;
        VD(usize)                                   selected_characteristics_index;
    } result;
} VD(VkPickPhysicalDeviceInfo);

VD(b32)     VDF(vk_require_instance_extensions) (VD(Arena) *temp, VD(VkRequireInstanceExtensionsInfo) *info);
VD(b32)     VDF(vk_require_instance_layers)     (VD(Arena) *temp, VD(VkRequireInstanceLayersInfo) *info);
void        VDF(vk_create_instance_extended)    (VD(VkCreateInstanceExtendedInfo) *info);
VD(b32)     VDF(vk_pick_physical_device)        (VD(Arena) *temp, VD(VkPickPhysicalDeviceInfo) *info);

#endif // !VD_VK_H

#ifdef VD_VK_IMPL
VD(b32) VDF(vk_require_instance_extensions)(VD(Arena) *temp, VD(VkRequireInstanceExtensionsInfo) *info)
{
    VD(u32) num_instance_extensions;
    VkExtensionProperties *instance_extensions = 0;

    // Get all available extensions
    VD_VK_CHECK(vkEnumerateInstanceExtensionProperties(0, &num_instance_extensions, 0));
    instance_extensions = VD_ARENA_PUSH_ARRAY(temp, VkExtensionProperties, num_instance_extensions);
    VD_VK_CHECK(vkEnumerateInstanceExtensionProperties(0, &num_instance_extensions, instance_extensions));

    if (info->log) {
        for (VD(usize) i = 0; i < info->count; ++i) {
            VD_DBGF("Required Extension       [%2zu] = %s", i, info->extensions[i]);
        }

        for (VD(usize) i = 0; i < num_instance_extensions; ++i) {
            VD_DBGF("Vulkan Instance extension[%2zu] = %s", i, instance_extensions[i].extensionName);
        }
    }

    // Check each require extension against all available ones and make sure they exist
    for (VD(usize) i = 0; i < info->count; ++i) {
        VD(b32) found = VD_FALSE;
        for (VD(usize) j = 0; j < num_instance_extensions; ++j) {
            if (VDF(cstr_cmp)(info->extensions[i], instance_extensions[j].extensionName)) {
                found = VD_TRUE;
                break;
            }
        }

        if (!found) return VD_FALSE;
    }

    return VD_TRUE;
}


VD(b32) VDF(vk_require_instance_layers)(VD(Arena) *temp, VD(VkRequireInstanceLayersInfo) *info)
{
    VD(u32) num_instance_layers;
    VkLayerProperties *instance_layers = 0;

    // Get all available extensions
    VD_VK_CHECK(vkEnumerateInstanceLayerProperties(&num_instance_layers, 0));
    instance_layers = VD_ARENA_PUSH_ARRAY(temp, VkLayerProperties, num_instance_layers);
    VD_VK_CHECK(vkEnumerateInstanceLayerProperties(&num_instance_layers, instance_layers));

    // Check each require layer against all available ones and make sure they exist
    for (VD(usize) i = 0; i < info->count; ++i) {
        VD(b32) found = VD_FALSE;
        for (VD(usize) j = 0; j < num_instance_layers; ++j) {
            if (VDF(cstr_cmp)(info->layers[i], instance_layers[j].layerName)) {
                found = VD_TRUE;
                break;
            }
        }

        if (!found) return VD_FALSE;
    }

    return VD_TRUE;
}

void VDF(vk_create_instance_extended)(VD(VkCreateInstanceExtendedInfo) *info)
{
    VkInstance result;

    VD_VK_CHECK(vkCreateInstance(& (VkInstanceCreateInfo){
        .sType                      = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .flags                      = info->flags,
        .pApplicationInfo = & (VkApplicationInfo) {
            .sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName       = info->app_name,
            .applicationVersion     = info->app_version,
            .pEngineName            = info->engine_name,
            .engineVersion          = info->engine_version,
            .apiVersion             = info->api_version,
        },
        .enabledLayerCount          = info->num_instance_layers,
        .ppEnabledLayerNames        = (const char * const *)info->instance_layers,
        .enabledExtensionCount      = info->num_instance_extensions,
        .ppEnabledExtensionNames    = (const char * const *)info->instance_extensions,
    }, 0, &result));

    *info->result.instance = result;

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
    }
}

VD(b32) VDF(vk_pick_physical_device)(VD(Arena) *temp, VD(VkPickPhysicalDeviceInfo) *info)
{
    // Get all physical devices
    VD(u32) num_phyiscal_devices;
    vkEnumeratePhysicalDevices(info->instance, &num_phyiscal_devices, 0);
    VkPhysicalDevice *physical_devices = VD_ARENA_PUSH_ARRAY(temp, VkPhysicalDevice, num_phyiscal_devices);
    vkEnumeratePhysicalDevices(info->instance, &num_phyiscal_devices, physical_devices);

    for (VD(u32) i = 0; i < num_phyiscal_devices; ++i) {
        VD(ArenaSave) save = VDF(arena_save)(temp);

        // Get all physical device extensions
        VD(u32) num_device_extensions;
        vkEnumerateDeviceExtensionProperties(physical_devices[i], 0, &num_device_extensions, 0);
        VkExtensionProperties *device_extension_properties = VD_ARENA_PUSH_ARRAY(temp, VkExtensionProperties, num_device_extensions);
        vkEnumerateDeviceExtensionProperties(physical_devices[i], 0, &num_device_extensions, device_extension_properties);

        // Parse physical device extensions into plain cstr array
        VD(cstr) *device_extensions = VD_ARENA_PUSH_ARRAY(temp, VD(cstr), num_device_extensions);
        for (VD(u32) j = 0; j < num_device_extensions; ++j) {
            device_extensions[j] = device_extension_properties[j].extensionName;
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
        VD(VkQueueSetup) *queues = VD_ARENA_PUSH_ARRAY(temp, VD(VkQueueSetup), num_queue_families);
        for (u32 j = 0; j < num_queue_families; ++j) {
            VD(VkGetPhysicalDevicePresentationSupportInfo) get_present_support_info = {
                .instance = info->instance,
                .physical_device = physical_devices[i],
                .queue_family_index = j,
            };
            queues[j] = (VD(VkQueueSetup)) {
                .capabilities = 
                    (queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)                     ? VD_(VK_QUEUE_CAPABILITIES_GRAPHICS) : 0 |
                    (queue_families[j].queueFlags & VK_QUEUE_TRANSFER_BIT)                     ? VD_(VK_QUEUE_CAPABILITIES_TRANSFER) : 0 |
                    (queue_families[j].queueFlags & VK_QUEUE_COMPUTE_BIT)                      ? VD_(VK_QUEUE_CAPABILITIES_COMPUTE)  : 0 |
                    info->get_physical_device_presentation_support(&get_present_support_info)  ? VD_(VK_QUEUE_CAPABILITIES_PRESENT)  : 0,
                .present_count = queue_families[j].queueCount,
                .cached_index = j,
            };
        }

        // Create @VkPhysicalDeviceCharacteristics
        VkPhysicalDeviceCharacteristics characteristics = {
            .multi_draw_indirect = features.features.multiDrawIndirect,
            .buffer_device_addr  = features12.bufferDeviceAddress,
            .descriptor_indexing = features12.descriptorIndexing,
            .synchronization2    = features13.synchronization2,
            .dynamic_rendering   = features13.dynamicRendering,
            .num_extensions      = num_device_extensions,
            .extensions          = device_extensions,
            .num_queue_setups    = num_queue_families,
            .queue_setups        = queues,
            .physical_device     = physical_devices[i],
        };

        VDF(arena_restore)(save);
    }

    return VD_FALSE;
}
#endif // VD_VK_IMPL