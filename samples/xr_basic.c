#include "vd_xr.h"
// #include "openxr_generated/openxr.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    uint32_t api_layer_count;
    XrResult result = xrEnumerateApiLayerProperties(0, &api_layer_count, 0);

    uint32_t count = 0;
    result = xrEnumerateInstanceExtensionProperties(NULL,
                                                    0,
                                                    &count,
                                                    NULL);
    return 0;
}

#define VD_XR_IMPL
#include "vd_xr.h"
