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

typedef struct __VD_VK_QueueGroup {
    VD(u32)     queue_count;
    VD(VkQueue) *queues;
} VD(VkQueueGroup);

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
    VD(b32)                                         log;
    struct {
        VkPhysicalDevice                            *physical_device;
        VD(usize)                                   *selected_characteristics_index;
    } result;
} VD(VkPickPhysicalDeviceInfo);

typedef struct __VD_VK_CreateDeviceAndQueuesInfo {
    VkInstance                          instance;
    VD(VkPhysicalDeviceCharacteristics) *characteristics;

    struct {
        VkDevice                        *device;
        VD(u32)                         num_queue_groups;
        VD(VkQueueGroup)                *queue_groups;
    } result;
} VD(VkCreateDeviceAndQueuesInfo);

VD(b32)     VDF(vk_require_instance_extensions)         (VD(Arena) *temp, VD(VkRequireInstanceExtensionsInfo) *info);
VD(b32)     VDF(vk_require_instance_layers)             (VD(Arena) *temp, VD(VkRequireInstanceLayersInfo) *info);
void        VDF(vk_create_instance_extended)            (VD(VkCreateInstanceExtendedInfo) *info);
void        VDF(vk_log_physical_device_characteristics) (VD(VkPhysicalDeviceCharacteristics) *characteristics);
VD(b32)     VDF(vk_pick_physical_device)                (VD(Arena) *temp, VD(VkPickPhysicalDeviceInfo) *info);
void        VDF(vk_create_device_and_queues)            (VD(Arena) *temp, VD(VkCreateDeviceAndQueuesInfo) *info);

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

    VD(i32)             best_device_index   = -1;
    VD(i32)             best_device_rank    = -1;
    VkPhysicalDevice    best_device_physical_device;


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

            VkQueueCapabilities caps = 0;

            if (queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) caps |= VD_(VK_QUEUE_CAPABILITIES_GRAPHICS);
            if (queue_families[j].queueFlags & VK_QUEUE_TRANSFER_BIT) caps |= VD_(VK_QUEUE_CAPABILITIES_TRANSFER);
            if (queue_families[j].queueFlags & VK_QUEUE_COMPUTE_BIT) caps |= VD_(VK_QUEUE_CAPABILITIES_COMPUTE);
            if (info->get_physical_device_presentation_support(&get_present_support_info)) caps |= VD_(VK_QUEUE_CAPABILITIES_PRESENT);

            queues[j] = (VD(VkQueueSetup)) {
                .capabilities = caps,
                .present_count = queue_families[j].queueCount,
                .cached_index = 0,
            };
        }

        // Create @VkPhysicalDeviceCharacteristics
        VkPhysicalDeviceCharacteristics characteristics = {
            .multi_draw_indirect = features.features.multiDrawIndirect,
            .buffer_device_addr  = features12.bufferDeviceAddress,
            .descriptor_indexing = features12.descriptorIndexing,
            .synchronization2    = features13.synchronization2,
            .dynamic_rendering   = features13.dynamicRendering,
            .num_queue_setups    = num_queue_families,
            .queue_setups        = queues,
            .num_extensions      = num_device_extensions,
            .extensions          = device_extensions,
            .physical_device     = physical_devices[i],
        };

        if (info->log) {
            DBGF("vd_vk_pick_physical_device: Testing against candidate: %u", i);
            VDF(vk_log_physical_device_characteristics)(&characteristics);
        }

        // For each avaiable set, compare and rank it
        for (VD(usize) j = 0; j < info->num_compatible_characteristics; ++j)
        {
            if (info->log) {
                DBGF("vd_vk_pick_physical_device: Checking compatible characteristics: %zu", j);
            }

            VkPhysicalDeviceCharacteristics *comparand = &info->compatible_characteristics[j];
            VD(i32) rank = comparand->rank_baseline;
            VD(b32) is_available = VD_TRUE;
            comparand->physical_device = physical_devices[i];

            // Check capabilities
            #define CHECK_CAP(x) do { if (!(!comparand->x || characteristics.x)) is_available = VD_FALSE; } while (0)
            CHECK_CAP(multi_draw_indirect);
            CHECK_CAP(buffer_device_addr);
            CHECK_CAP(descriptor_indexing);
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
            for (VD(usize) k = 0; k < comparand->num_extensions; ++k) {
                VD(b32) found = VD_FALSE;
                for (VD(usize) z = 0; z < num_device_extensions; ++z) {
                    if (VD(cstr_cmp)(device_extension_properties[z].extensionName, comparand->extensions[k])) {
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

            // Check queues
            for (VD(usize) k = 0; k < comparand->num_queue_setups; ++k) {
                VD(VkQueueSetup) *want_queue_family = &comparand->queue_setups[k];
                VD(i32) queue_index = -1;

                // Loop over all available queues
                for (VD(usize) z = 0; z < characteristics.num_queue_setups; ++z) {

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
                        queue_index = (VD(i32))z;
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
                best_device_index = j;
                best_device_physical_device = physical_devices[i];
                best_device_rank = rank;
            }
        }

        VDF(arena_restore)(save);
    }

    if (best_device_index < 0) {
        return VD_FALSE;
    }

    *info->result.physical_device = best_device_physical_device;
    *info->result.selected_characteristics_index = (VD(i32))best_device_index;

    return VD_TRUE;
}

void VDF(vk_log_physical_device_characteristics)(VD(VkPhysicalDeviceCharacteristics) *characteristics)
{
    LOGF("%-30s", "Capabilities---------");
    LOGF("%-30s= %d", "multi_draw_indirect",  characteristics->multi_draw_indirect);
    LOGF("%-30s= %d", "buffer_device_addr",   characteristics->buffer_device_addr);
    LOGF("%-30s= %d", "descriptor_indexing",  characteristics->descriptor_indexing);
    LOGF("%-30s= %d", "synchronization2",     characteristics->synchronization2);
    LOGF("%-30s= %d", "dynamic_rendering",    characteristics->dynamic_rendering);
    LOGF("%-30s= %d", "prefer_discrete",      characteristics->prefer_discrete);
    LOGF("%-30s= %d", "prefer_integrated",    characteristics->prefer_integrated);

    LOGF("%-30s", "Extensions-----------");
    for (VD(usize) i = 0; i < characteristics->num_extensions; ++i)
        LOGF("%-30s", characteristics->extensions[i]);

    LOGF("%-30s", "Queue Setups---------");
    for (VD(usize) i = 0; i < characteristics->num_queue_setups; ++i) {
        LOGF("[%zu].required_count: %u", i, characteristics->queue_setups[i].required_count);
        LOGF("[%zu].present_count:  %u", i, characteristics->queue_setups[i].present_count);
        LOGF("[%zu].cached_index:   %d", i, characteristics->queue_setups[i].cached_index);
        LOGF("%s", "                      Graphics Compute Transfer Present");
        LOGF("[%zu].capabilities:     %d        %d       %d        %d",
            i,
            characteristics->queue_setups[i].capabilities & VD_(VK_QUEUE_CAPABILITIES_GRAPHICS) ? 1 : 0,
            characteristics->queue_setups[i].capabilities & VD_(VK_QUEUE_CAPABILITIES_COMPUTE)  ? 1 : 0,
            characteristics->queue_setups[i].capabilities & VD_(VK_QUEUE_CAPABILITIES_TRANSFER) ? 1 : 0,
            characteristics->queue_setups[i].capabilities & VD_(VK_QUEUE_CAPABILITIES_PRESENT   ? 1 : 0));
    }

}

void VDF(vk_create_device_and_queues)(VD(Arena) *temp, VD(VkCreateDeviceAndQueuesInfo) *info)
{
    VD_ASSERT(info->characteristics->num_queue_setups == info->result.num_queue_groups);

    VD(u32) num_queue_groups = (VD(u32))info->characteristics->num_queue_setups;

    // Create arrays for VkDeviceQueueCreateInfo
    VkDeviceQueueCreateInfo *queue_create_infos = ARENA_PUSH_ARRAY(temp, VkDeviceQueueCreateInfo, num_queue_groups); 
    for (VD(u32) i = 0; i < num_queue_groups; ++i) {
        VD_ASSERT(info->characteristics->queue_setups[i].required_count == info->result.queue_groups[i].queue_count);
        VD_ASSERT(info->characteristics->queue_setups[i].cached_index >= 0);

        VD(u32) queue_count = info->characteristics->queue_setups[i].required_count;

        float *priorities = ARENA_PUSH_ARRAY(temp, float, queue_count);
        for (VD(u32) j = 0; j < queue_count; ++j) {
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
        .sType                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .bufferDeviceAddress    = info->characteristics->buffer_device_addr,
        .descriptorIndexing     = info->characteristics->descriptor_indexing,
    };

    VkPhysicalDeviceVulkan13Features features13 = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &features12,
        .synchronization2       = info->characteristics->synchronization2,
        .dynamicRendering       = info->characteristics->dynamic_rendering,
    };

    VkPhysicalDeviceFeatures2 features = {
        .sType                  = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext                  = &features13,
        .features               = {
            .multiDrawIndirect  = info->characteristics->multi_draw_indirect,
        },
    };

    VD_VK_CHECK(vkCreateDevice(info->characteristics->physical_device, & (VkDeviceCreateInfo) {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount    = info->characteristics->num_queue_setups,
        .pQueueCreateInfos       = queue_create_infos,
        .enabledExtensionCount   = info->characteristics->num_extensions,
        .ppEnabledExtensionNames = (const char * const *)info->characteristics->extensions,
        .pNext                   = &features,
    }, 0, info->result.device));

    for (VD(u32) i = 0; i < num_queue_groups; ++i) {
        for (VD(u32) q = 0; q < info->result.queue_groups[i].queue_count; ++q) {
            vkGetDeviceQueue(
                *info->result.device,
                info->characteristics->queue_setups[i].cached_index, 
                q,
                &info->result.queue_groups[i].queues[q]);
        }
    }
}

#endif // VD_VK_IMPL