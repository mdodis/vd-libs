#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <threads.h>
#include <stdint.h>
#include "vd_au.h"
#include "vd_fw.h"

#define WAVE_ID_RIFF           0x46464952
#define WAVE_MASTER_FORMAT_ID  0x45564157
#define WAVE_ID_FORMAT         0x20746d66
#define WAVE_ID_FORMAT_EX      0xFFFE
#define WAVE_ID_DATA           0x61746164

typedef struct {
    FILE        *f;
    int         is_integer;
    int         bits;
    int         rate;
    int         channels;
    int         currently_in_chunk;
    uint32_t    curr_chunk_remaining;
} AudioSource;

static AudioSource  Audio_Source;
static VdAuEndpoint Audio_Output;
static size_t       Buffer_Size;
static float        *Output_Buffer;
uint8_t read_buf[4096];
float   resample_buf[44'100];
float   source_buf[16'000];

uint32_t resample_hermite_input_count_from_output_count(uint32_t output_count, float scale)
{
    return (uint32_t)ceilf(((float)output_count * (1.f / scale)));
}

uint32_t resample_hermite(float *input, int input_channels, uint32_t input_frames, float *output, float scale)
{
    uint32_t output_frames = (uint32_t)ceilf(input_frames * scale);

    if (output) {

        for (uint32_t i = 0; i < output_frames; ++i) {

            float e = (float)i / (float)output_frames; 

            uint32_t b_i = (uint32_t)(e * (float)input_frames);
            uint32_t a_i = b_i - 1;
            uint32_t c_i = b_i + 1;
            uint32_t d_i = b_i + 2;

            float t = (float)(e * input_frames) - (float)(b_i);
            for (int channel = 0; channel < input_channels; ++channel) {
                uint32_t a_ic = a_i * input_channels + channel;
                uint32_t b_ic = b_i * input_channels + channel;
                uint32_t c_ic = c_i * input_channels + channel;
                uint32_t d_ic = d_i * input_channels + channel;

                float a = a_i >= 0 && a_i < input_frames ? input[a_ic] : input[b_ic];
                float b = input[b_ic];
                float c = c_i >= 0 && c_i < input_frames ? input[c_ic] : input[b_ic];  
                float d = d_i >= 0 && d_i < input_frames ? input[d_ic] : input[b_ic];  

                float c0 = b;
                float c1 = (c - a) * 0.5f;
                float c2 = a - (b * 2.5f) + (c * 2.0f) - (d * 0.5f);
                float c3 = ((b - c) * 1.5f) + ((d - a) * 0.5f);
                output[i * input_channels + channel] = (((((c3 * t) + c2) * t) + c1) * t) + c0;
            }
        }
    }

    return output_frames;
}

#pragma pack(push, 1)
typedef struct {
    uint32_t    id;
    uint32_t    size;
} WaveChunkHeader;

typedef struct {
    WaveChunkHeader header;
    uint32_t        file_format_id;
} WaveMasterChunk;

typedef struct {
    uint16_t        format;
    uint16_t        channels;
    uint32_t        sample_rate;
    uint32_t        bytes_per_sec;
    uint16_t        bytes_per_bloc;
    uint16_t        bits_per_sample;
} WaveFormatChunk;

typedef struct {
    WaveFormatChunk format;
    uint16_t        size;
} WaveFormatExChunk;

typedef struct {
    uint32_t        guid_data1;
    uint16_t        guid_data2;
    uint16_t        guid_data3;
    uint8_t         guid_data4[8];
} WaveGuid;

WaveGuid Wave_Sub_Format_PCM       = {0x00000001, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71};
WaveGuid Wave_Sub_Format_IEEEFloat = {0x00000000, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71};

static int wave_guid_cmp(WaveGuid *a, WaveGuid *b)
{
    return memcmp(a, b, sizeof(*a)) == 0;
}

typedef struct {
    WaveFormatExChunk   ex;
    uint16_t            samples_per_bloc;
    uint32_t            channel_mask;
    WaveGuid            sub_format;
} WaveFormatExtensibleChunk;

#pragma pack(pop)


float clampf(float min, float x, float max)
{
    x = x > max ? max : x;
    return x < min ? min : x;
}

int audio_thread_proc(void *_arg)
{
    (void)_arg;

    while (1) {
        int frames_available = vd_au_endpoint_frames_available(&Audio_Output);
        uint8_t *ptr = 0;
        VdAuStat stat = vd_au_endpoint_write_begin(&Audio_Output, frames_available, &ptr);
        if (stat == VD_AU_STAT_OK) {

            float scale = (float)Audio_Output.frame_rate / (float)Audio_Source.rate;

            uint32_t source_frames_to_read = resample_hermite_input_count_from_output_count(frames_available, scale);

            // Read Audio Chunks
            uint32_t source_frames_read = 0;
            while (source_frames_read < source_frames_to_read) {

                uint32_t channel_stride = Audio_Source.channels;
                uint32_t sample_stride = (Audio_Source.bits / 8);
                uint32_t data_size = (source_frames_to_read - source_frames_read) * channel_stride * sample_stride;

                if (!Audio_Source.currently_in_chunk) {
                    WaveChunkHeader hdr;
                    if (!fread(&hdr, sizeof(hdr), 1, Audio_Source.f)) {
                        break;
                    }

                    if (hdr.id != WAVE_ID_DATA) {
                        // Skip chunk
                        fseek(Audio_Source.f, hdr.size, SEEK_CUR);
                        continue;
                    }

                    Audio_Source.currently_in_chunk = 1;
                    Audio_Source.curr_chunk_remaining = hdr.size;
                }

                if (Audio_Source.curr_chunk_remaining < data_size) {
                    data_size = Audio_Source.curr_chunk_remaining;
                }

                if (data_size > sizeof(read_buf)) {
                    data_size = sizeof(read_buf);
                }

                if (fread(read_buf, data_size, 1, Audio_Source.f)) {
                    // Actually process audio here

                    uint32_t frames_in = data_size / (sample_stride * channel_stride);

                    for (uint32_t f = 0; f < frames_in; ++f) {
                        for (uint32_t c = 0; c < channel_stride; ++c) {
                            uint8_t *sample_in  = &read_buf[f * sample_stride * channel_stride + c * sample_stride];
                            float   *sample_out = &source_buf[source_frames_read * channel_stride + f * channel_stride + c];

                            switch (sample_stride) {
                                case 2: {
                                    // 16 bit
                                    int16_t sample = *(int16_t*)sample_in;

                                    float value = clampf(-1.f, ((float)sample / 32767.f), 1.f);

                                    *sample_out = value;
                                } break;
                                default: break;
                            }
                        }
                    }
                    source_frames_read += frames_in;

                    Audio_Source.curr_chunk_remaining -= data_size;
                    Audio_Source.currently_in_chunk = Audio_Source.curr_chunk_remaining > 0;
                } else {
                    break;
                }
                
            }
            uint32_t output_frames_produced = resample_hermite(source_buf, Audio_Source.channels, source_frames_read,
                                                               (float*)ptr, scale);

            while (output_frames_produced < (uint32_t)frames_available) {
                ((float*)ptr)[output_frames_produced++] = 0.f;
            }

            vd_au_endpoint_write_end(&Audio_Output);
        } else if (stat > 0) {
            break;
        } else {
            thrd_yield();
        }
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Please specify path to wave file.\n");
        return -1;
    }

    FILE *f = fopen(argv[1], "rb");

    WaveMasterChunk first_chunk;
    if (fread(&first_chunk, sizeof(first_chunk), 1, f)) {

        if (first_chunk.header.id != WAVE_ID_RIFF) {
            printf("File is not a RIFF file.\n");
            return -1;
        }

        if (first_chunk.file_format_id != WAVE_MASTER_FORMAT_ID) {
            printf("File is a RIFF file but not a WAVE file.\n");
            return -1;
        }
    } else {
        printf("Reading master chunk failed. Probably not a wave file\n");
        return -1;
    }

    WaveChunkHeader next_chunk;
    if (fread(&next_chunk, sizeof(next_chunk), 1, f)) {
        if (next_chunk.id != WAVE_ID_FORMAT) {
            printf("Expected format chunk, found %8x\n", next_chunk.id);
            return -1;
        }
    }

    WaveFormatChunk format_chunk;
    fread(&format_chunk, sizeof(format_chunk), 1, f);

    if (format_chunk.format == WAVE_ID_FORMAT_EX) {
        printf("Microsoft Wave Ex Format\n");
        fseek(f, -((long)sizeof(format_chunk)), SEEK_CUR);

        WaveFormatExtensibleChunk ex_chunk;
        fread(&ex_chunk, sizeof(ex_chunk), 1, f);

        if (wave_guid_cmp(&ex_chunk.sub_format, &Wave_Sub_Format_IEEEFloat)) {
            printf("IEEE Float sub-format\n");
            Audio_Source.is_integer = 0;
        } else if (wave_guid_cmp(&ex_chunk.sub_format, &Wave_Sub_Format_PCM)) {
            printf("PCM sub-format\n");
            Audio_Source.is_integer = 1;
        }
    }
    Audio_Source.bits       = format_chunk.bits_per_sample;
    Audio_Source.channels   = format_chunk.channels;
    Audio_Source.rate       = format_chunk.sample_rate;

    Audio_Source.f = f;
    Audio_Source.currently_in_chunk = 0;
    vd_fw_init(0);

    vd_au_init();
    Audio_Output = vd_au_endpoint_default();

    Buffer_Size = sizeof(float) * Audio_Output.channels * Audio_Output.frame_count;
    Output_Buffer = (float*)malloc(Buffer_Size);
    memset(Output_Buffer, 0, Buffer_Size);

    vd_au_endpoint_start(&Audio_Output);

    thrd_t audio_thread_handle;
    thrd_create(&audio_thread_handle, audio_thread_proc, 0);

    while (vd_fw_running()) {
        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        vd_fw_unlock();
    }

    vd_au_endpoint_stop(&Audio_Output);

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_AU_IMPL
#include "vd_au.h"
