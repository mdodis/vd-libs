#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vd_au.h"


#define PI2 (2.f * 3.14159265359f)
static void slp(uint32_t ms);
static int should_stop(void);
int main(int argc, char const *argv[])
{
    vd_au_init();
    VdAuEndpoint output = vd_au_endpoint_default();

    size_t buffer_size = sizeof(float) * output.channels * output.frame_count;

    float *buf = (float*)malloc(buffer_size);
    memset(buf, 0, buffer_size);

    // Silence
    {
        uint8_t *ptr = 0;
        vd_au_endpoint_write_begin(&output, output.frame_count, &ptr);
        memcpy(ptr, buf, buffer_size);
        vd_au_endpoint_write_end(&output);
    }

    float frequency = 440.f;
    float amplitude = 0.3f;

    float t = 0.f;

    vd_au_endpoint_start(&output);

    while (1) {
        int frames_available = vd_au_endpoint_frames_available(&output);

        uint8_t *ptr = 0;
        VdAuStat stat = vd_au_endpoint_write_begin(&output, frames_available, &ptr);

        if (stat == VD_AU_STAT_OK) {
            float *out = (float*)ptr;

            for (int f = 0; f < frames_available; ++f) {

                for (int c = 0; c < output.channels; ++c) {
                    float v = sinf(PI2 * frequency * t) * amplitude;

                    out[f * output.channels + c] = v;

                }

                t += (1.f / (float)output.frame_rate);
            }

            vd_au_endpoint_write_end(&output);
        } else if (stat > 0) {
            break;
        }

        while (t > 1.f) {
            t -= 1.f;
        }

        if (should_stop()) {
            break;
        }
    }


    vd_au_endpoint_stop(&output);

    return 0;
}

#define VD_AU_IMPL
#include "vd_au.h"

static void slp(uint32_t ms)
{
    Sleep(ms);
}

#pragma comment(lib, "User32.lib")
static int should_stop(void)
{
    return GetAsyncKeyState(VK_ESCAPE);
}
