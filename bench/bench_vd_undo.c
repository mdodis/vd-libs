#include <string.h>
#include <intrin.h>
#include "vd_undo.h"

static uint8_t *state_1;
static uint8_t *state_2;
static uint8_t *buf;
static uint32_t state_size;
static uint32_t buffer_cap;
static uint32_t loops;
static uint32_t mutcount;
static uint32_t skipcount;
static VdUndo  undo;

static void s1_record(uint32_t loops, uint32_t mutation_count, uint32_t skip_count, int output);


int main(int argc, char const *argv[])
{
    if (argc != (1 + 5)) {
        printf("Invocation: <state size> <buffer cap> <loops> <mutcount> <skipcount>\n");
        return -1;
    }

    state_size = strtoul(argv[1], 0, 10);
    buffer_cap = strtoul(argv[2], 0, 10);
    loops      = strtoul(argv[3], 0, 10);
    mutcount   = strtoul(argv[4], 0, 10);
    skipcount  = strtoul(argv[5], 0, 10);

    buf = calloc(1, buffer_cap);
    state_1 = calloc(1, state_size);
    state_2 = calloc(1, state_size);
    undo = vd_undo_make(buf, buffer_cap, state_size);
    // Warmup
    s1_record(100, 10, 8, 0);

    s1_record(loops, mutcount, skipcount, 1);
    return 0;
}

static void s1_record(uint32_t loops, uint32_t mutation_count, uint32_t skip_count, int output)
{
    memset(state_1, 0, state_size);

    int64_t cy_diff_total = 0;
    for (uint32_t i = 0; i < loops; ++i) {
        memcpy(state_2, state_1, state_size);

        for (uint32_t j = 0; j < mutation_count; ++j) {
            state_1[(j + i + skip_count) % state_size] += 1;
        }

        _mm_lfence();
        int64_t cy_begin = __rdtsc();
        {
            vd_undo_invalidate(&undo, state_2, state_1, state_size);
        }
        _mm_lfence();
        int64_t cy_end = __rdtsc();

        int64_t cy_diff = cy_end - cy_begin;
        cy_diff_total += cy_diff;
    }

    if (output) {
        double avg_cy_per_check = (double)cy_diff_total / (double)loops;
        double avg_cy_per_state  = avg_cy_per_check / (double)(state_size);

        // printf("\n");
        // printf("%s,%s,%s,%s,%s,%s,%s,\n", "Scenario 1", "State Size", "Buffer Cap", "Loops", "Mut. Count", "Skip Count", "cy/state");
        printf("%s,%u,%u,%u,%u,%u,%.2f,%.2f\n", "", state_size, buffer_cap, loops, mutation_count, skip_count, avg_cy_per_state, avg_cy_per_check);
        // printf("%s%.2f\n", "AVG cy/check", avg_cy_per_check);
        // printf("%s%.2f\n", "AVG cy/byte", avg_cy_per_byte);
    }
}

#define VD_UNDO_IMPL
#include "vd_undo.h"
