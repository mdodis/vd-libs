/**
 * windisplays.c - Win32 Display Monitor Enumeration Sample
 * Author: Michael Dodis (michaeldodisgr@gmail.com)
 * 
 * Based on ESA ENHANCED EXTENDED DISPLAY IDENTIFICATION DATA STANDARD
 * Release A, Revision 2
 * September 25, 2006
 * ---------------------------------------------------------------------------------------------------------------------
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 * ---------------------------------------------------------------------------------------------------------------------
 */
#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <stdint.h>
#include <assert.h>
#include <physicalmonitorenumerationapi.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Dxva2.lib")

#pragma pack(push, 1)
typedef struct {
    uint8_t                                     horizontal_addressable_pixels;                      // (horizontal_addressable_pixels + 31) * 8 
    uint8_t                                     field_refresh_rate : 6;
    uint8_t                                     image_aspect_ratio : 2;
} Edid1_4StandardTiming;

typedef struct {
    // Detailed Timing Definitions - Part 1
    uint16_t                                    pixel_clock;                                        // pixel_clock * 10000, 0x0000 --> This is a Display Descriptor
    uint8_t                                     horizontal_addressable_pixels_lo;
    uint8_t                                     horizontal_blanking_pixels_lo;
    uint8_t                                     horizontal_addressable_pixels_hi : 4;
    uint8_t                                     horizontal_blanking_pixels_hi : 4;
    uint8_t                                     vertical_addressable_lines_lo;
    uint8_t                                     vertical_blanking_lines_lo;
    uint8_t                                     vertical_addressable_lines_hi : 4;
    uint8_t                                     vertical_blanking_lines_hi : 4;
    uint8_t                                     horizontal_front_porch_pixels_lo;
    uint8_t                                     horizontal_sync_pulse_width_pixels_lo;
    uint8_t                                     vertical_front_porch_lines_lo : 4;
    uint8_t                                     vertical_sync_pulse_width_lines_lo : 4;

    // Bit Definitions
    uint8_t                                     horizontal_front_porch_pixels_hi : 2;
    uint8_t                                     horizontal_sync_pulse_width_pixels_hi : 2;
    uint8_t                                     vertical_front_porch_lines_hi : 2;
    uint8_t                                     vertical_sync_pulse_width_lines_hi : 2;

    // Video Image Size & Border Definitions
    uint8_t                                     horizontal_addressable_video_image_size_mm_lo;
    uint8_t                                     vertical_addressable_video_image_size_mm_lo;
    uint8_t                                     horizontal_addressable_video_image_size_mm_hi : 4;
    uint8_t                                     vertical_addressable_video_image_size_mm_hi : 4;
    uint8_t                                     right_left_horizontal_border_pixels;
    uint8_t                                     top_bottom_vertical_border_lines;

    // Detailed Timing Definitions - Part 2
    uint8_t                                     signal_interface_type : 1;
    uint8_t                                     stereo_viewing_support_hi : 2;
    uint8_t                                     a_d_sync_signal_definitions : 4;
    uint8_t                                     stereo_viewing_support_lo : 1;
} Edid1_4DetailedTiming;

typedef struct {
    uint8_t                                     bytes[13];
} Edid1_4DisplayDescriptorDataOpaque;

typedef struct {
    char                                        serial_number[13];
} Edid1_4DisplayProductSerialNumber;

typedef struct {
    char                                        name[13];
} Edid1_4DisplayProductName;

typedef union {
    Edid1_4DisplayDescriptorDataOpaque          opaque;         // 13 bytes
    Edid1_4DisplayProductSerialNumber           serial_number;  // 0xFF
    Edid1_4DisplayProductName                   product_name;   // 0xFC
} Edid1_4DisplayDescriptorDataBlock;

typedef struct {
    uint16_t                                    zeroes;
    uint8_t                                     zero;
    uint8_t                                     tag;
    uint8_t                                     zero_or_display_range_limits_descriptor; 
    Edid1_4DisplayDescriptorDataBlock           data_block;
} Edid1_4DisplayDescriptor;

typedef union {
    Edid1_4DetailedTiming                       detailed_timing;
    Edid1_4DisplayDescriptor                    display_descriptor;
} Edid1_4DataBlock;

typedef struct {
    uint8_t                                     digital_signal_bit : 1;
    uint8_t                                     signal_level_standard : 2;
    uint8_t                                     video_setup : 1;
    uint8_t                                     sync_types : 3;
    uint8_t                                     serrations : 1;
} Edid1_4AnalogVideoSignalDefinition;

typedef struct {
    uint8_t                                     digital_signal_bit : 1;
    uint8_t                                     color_bit_depth : 3;
    uint8_t                                     video_interface_standard_supported : 4;
} Edid1_4DigitalVideoSignalDefinition;

typedef struct {
    uint8_t                                     digital_signal_bit : 1;
    uint8_t                                     rest : 7;
} Edid1_4UndeterminedVideoSignalDefinition;

typedef union {
    Edid1_4UndeterminedVideoSignalDefinition    general;
    Edid1_4AnalogVideoSignalDefinition          analog;
    Edid1_4DigitalVideoSignalDefinition         digital;
} Edid1_4VideoInputDefinition;

typedef struct {
    uint8_t                                     header[8];                                      // 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00

    // Vendor & Product Identification
    uint16_t                                    id_manufacturer_name;                           // Section 3.4.1
    uint16_t                                    id_product_code;                                // Section 3.4.2
    uint32_t                                    id_serial_number;                               // Section 3.4.3
    uint8_t                                     week_of_manufacture;                            // Section 3.4.4
    uint8_t                                     year_of_manufacture;                            // Section 3.4.4

    // EDID Structure Version & Revision
    uint8_t                                     version;
    uint8_t                                     revision;

    // Basic Display Parameters
    Edid1_4VideoInputDefinition                 video_input_definition;                         // Section 3.6.1
    uint8_t                                     horizontal_image_size_or_aspect;                // cm.
    uint8_t                                     vertical_image_size_or_aspect;                  // cm.
    uint8_t                                     display_transfer_characteristic;                // Gamma
    uint8_t                                     feature_support;                                // Section 3.6.4

    // Color Characteristics
    uint8_t                                     red_green_low_order_bits;
    uint8_t                                     blue_white_low_order_bits;
    uint8_t                                     red_x_high_order_bits;
    uint8_t                                     red_y_high_order_bits;
    uint8_t                                     green_x_high_order_bits;
    uint8_t                                     green_y_high_order_bits;
    uint8_t                                     blue_x_high_order_bits;
    uint8_t                                     blue_y_high_order_bits;
    uint8_t                                     white_x_high_order_bits;
    uint8_t                                     white_y_high_order_bits;

    // Established Timings
    uint8_t                                     established_timings_1;
    uint8_t                                     established_timings_2;
    uint8_t                                     manufacturer_reserved_timings;

    // Standard Timings: Identification 1 -> 8
    Edid1_4StandardTiming                       standard_timings[8];
    
    // 18 Byte Data Blocks
    Edid1_4DataBlock                            data_blocks[4];

    uint8_t                                     extension_block_count;
    uint8_t                                     checksum;

} Edid1_4;
#pragma pack(pop)

static uint16_t swapu16(uint16_t x) {
    return (x << 8) | (x >> 8);
}

static int parse_edid(BYTE *data, DWORD data_size)
{
    assert(sizeof(Edid1_4VideoInputDefinition) == 1);
    assert(sizeof(Edid1_4DetailedTiming) == 18);
    assert(sizeof(Edid1_4DisplayDescriptor) == 18);
    assert(sizeof(Edid1_4DataBlock) == 18);
    if (data_size < sizeof(Edid1_4)) {
        return 0;
    }

    Edid1_4 *edid = (Edid1_4*)data;

    if (edid->header[0] != 0x00) return 0;
    if (edid->header[1] != 0xFF) return 0;
    if (edid->header[2] != 0xFF) return 0;
    if (edid->header[3] != 0xFF) return 0;
    if (edid->header[4] != 0xFF) return 0;
    if (edid->header[5] != 0xFF) return 0;
    if (edid->header[6] != 0xFF) return 0;
    if (edid->header[7] != 0x00) return 0;

    printf("\tVendor & Product Identification\n");
    printf("\t\tProduct Code: 0x%04x\n", edid->id_product_code);
    printf("\t\tSerial Number: %d\n", edid->id_serial_number);
    printf("\t\tManufacture Date: %d/%d\n", edid->week_of_manufacture, edid->year_of_manufacture);

    printf("\tEDID Structure Version & Revision\n");
    printf("\t\tEDID Version: %d.%d\n", edid->version, edid->revision);

    printf("\tBasic Display Parameters\n");
    if (edid->video_input_definition.general.digital_signal_bit == 0) {
        printf("\t\tAnalog Video Signal Interface\n");
        switch (edid->video_input_definition.analog.signal_level_standard) {
            case 0b00: printf("\t\tSignal Level Standard: 0.700 : 0.300 : 1.000 V p-p\n"); break;
            case 0b01: printf("\t\tSignal Level Standard: 0.714 : 0.286 : 1.000 V p-p\n"); break;
            case 0b10: printf("\t\tSignal Level Standard: 1.000 : 0.400 : 1.400 V p-p\n"); break;
            case 0b11: printf("\t\tSignal Level Standard: 0.700 : 0.000 : 0.700 V p-p\n"); break;
            default: break;
        }
    } else {
        printf("\t\tDigital Video Signal Interface\n");
        switch(edid->video_input_definition.digital.color_bit_depth) {
            case 0b000: printf("\t\tColor Bit Depth: Undefined\n"); break;
            case 0b001: printf("\t\tColor Bit Depth:  6 Bits per Primary Color\n"); break;
            case 0b010: printf("\t\tColor Bit Depth:  8 Bits per Primary Color\n"); break;
            case 0b011: printf("\t\tColor Bit Depth: 10 Bits per Primary Color\n"); break;
            case 0b100: printf("\t\tColor Bit Depth: 12 Bits per Primary Color\n"); break;
            case 0b101: printf("\t\tColor Bit Depth: 14 Bits per Primary Color\n"); break;
            case 0b110: printf("\t\tColor Bit Depth: 16 Bits per Primary Color\n"); break;
            default: break;
        }

        switch (edid->video_input_definition.digital.video_interface_standard_supported) {
            case 0b0000: printf("\t\tVideo Interface Standard: Undefined\n"); break;
            case 0b0001: printf("\t\tVideo Interface Standard: DVI\n"); break;
            case 0b0010: printf("\t\tVideo Interface Standard: HDMI-a\n"); break;
            case 0b0011: printf("\t\tVideo Interface Standard: HDMI-b\n"); break;
            case 0b0100: printf("\t\tVideo Interface Standard: MDDI\n"); break;
            case 0b0101: printf("\t\tVideo Interface Standard: DisplayPort\n"); break;
            default:     printf("\t\tVideo Interface Standard: Invalid (0x%x)\n", edid->video_input_definition.digital.video_interface_standard_supported); break;
        }
    }

    for (int i = 0; i < 4; ++i) {
        Edid1_4DataBlock *data_block = &edid->data_blocks[i];

        if (data_block->detailed_timing.pixel_clock == 0) {
            printf("\t\t\tDisplay Descriptor\n");

            Edid1_4DisplayDescriptor *display_descriptor = &data_block->display_descriptor;
            switch (display_descriptor->tag) {
                case 0xFF: {
                    printf("\t\t\t\tDisplay Product Serial Number\n");
                } break;

                case 0xFE: {
                    printf("\t\t\t\tAlphanumeric Data String (ASCII)\n");
                } break;

                case 0xFD: {
                    printf("\t\t\t\tDisplay Range Limits\n");
                } break;

                case 0xFC: {
                    int length = 0;
                    char *c = display_descriptor->data_block.product_name.name;
                    while ((length < 13) && (*c != '\r') && (*c != '\n')) {
                        length++;
                        c++;
                    }

                    printf("\t\t\t\tDisplay Product Name: %.*s\n", length, display_descriptor->data_block.product_name.name);
                } break;

                case 0xFB: {
                    printf("\t\t\t\tColor Point Data\n");
                } break;

                case 0xFA: {
                    printf("\t\t\t\tStandard Timing Identifications\n");
                } break;

                case 0xF9: {
                    printf("\t\t\t\tDCM Data\n");
                } break;

                case 0xF8: {
                    printf("\t\t\t\tCVT 3 Byte Timing Codes\n");
                } break;

                case 0xF7: {
                    printf("\t\t\t\tEstablished Timings 3\n");
                } break;

                case 0x10: {
                    printf("\t\t\t\tDummy Descriptor\n");
                } break;

                case 0x0F:
                case 0x0E:
                case 0x0D:
                case 0x0C:
                case 0x0B:
                case 0x0A:
                case 0x09:
                case 0x08:
                case 0x07:
                case 0x06:
                case 0x05:
                case 0x04:
                case 0x03:
                case 0x02:
                case 0x01:
                case 0x00: {
                    printf("\t\t\t\tManufacturer Specified Display Descriptors\n");
                } break;

                default: break;
            }
        } else {
            printf("\t\t\tDetailed Timing\n");
        }
    }

    printf("\t\tExtension Count: %d\n", edid->extension_block_count);
    printf("\t\tChecksum: %02x\n", edid->checksum);


    return 1;
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    GUID guid_devinterface_monitor = { 0xe6f07b5f, 0xee97, 0x4a90, 0xb0, 0x76, 0x33, 0xf5, 0x7b, 0xf4, 0xea, 0xa7 };

    int adapter_index = 0;
    DISPLAY_DEVICEW display_adapter = {0};
    display_adapter.cb = sizeof(display_adapter);
    while (EnumDisplayDevicesW(NULL, adapter_index, &display_adapter, 0)) {
        DISPLAY_DEVICEW display_monitor = {0};
        display_monitor.cb = sizeof(display_monitor);
        printf("::%S::\n", display_adapter.DeviceName);
        if (EnumDisplayDevicesW(display_adapter.DeviceName, 0, &display_monitor, EDD_GET_DEVICE_INTERFACE_NAME)) {
            printf("ID: %S\n", display_monitor.DeviceID);
            printf("Name: %S\n", display_monitor.DeviceName);
            printf("String: %S\n", display_monitor.DeviceString);
            printf("Key: %S\n", display_monitor.DeviceKey);

            DEVMODEW devmode;
            int graphics_mode_index = 0;
            while (EnumDisplaySettingsW(display_adapter.DeviceName, graphics_mode_index, &devmode)) {
                printf("- [%S] V%d:%d %dx%d@%d Orientation: %d BitsPerPel: %d Pos: <%d,%d>\n", devmode.dmDeviceName, devmode.dmSpecVersion, devmode.dmDriverVersion, devmode.dmPelsWidth, devmode.dmPelsHeight, devmode.dmDisplayFrequency, devmode.dmDisplayOrientation, devmode.dmBitsPerPel,
                    devmode.dmPosition.x, devmode.dmPosition.y);
                graphics_mode_index++;
            }

            HDEVINFO devinfo = SetupDiGetClassDevsW(&guid_devinterface_monitor, NULL, NULL, DIGCF_DEVICEINTERFACE);
            if (devinfo != INVALID_HANDLE_VALUE) {

                int di_index = 0;
                SP_DEVICE_INTERFACE_DATA di_data = {0};
                di_data.cbSize = sizeof(di_data);
                while (SetupDiEnumDeviceInterfaces(devinfo, NULL, &guid_devinterface_monitor, di_index, &di_data)) {

                    static char di_detail_data_buffer[512] = {0};

                    SP_DEVICE_INTERFACE_DETAIL_DATA_W *pdi_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA_W*)di_detail_data_buffer;
                    pdi_detail_data->cbSize = sizeof(*pdi_detail_data);

                    if (SetupDiGetDeviceInterfaceDetailW(devinfo, &di_data,
                                                         pdi_detail_data, 512, // DeviceInterfaceDetailDataSize
                                                         0, NULL))
                    {

                        if (_wcsicmp(pdi_detail_data->DevicePath, display_monitor.DeviceID) == 0) {
                            printf("\t- %S :: %S\n", pdi_detail_data->DevicePath, display_monitor.DeviceID);

                            SP_DEVINFO_DATA devinfo_data = {0};
                            devinfo_data.cbSize = sizeof(devinfo_data);
                            if (SetupDiEnumDeviceInfo(devinfo, di_index, &devinfo_data)) {
                                HKEY hk = SetupDiOpenDevRegKey(devinfo, &devinfo_data, 
                                                               DICS_FLAG_GLOBAL, 0, DIREG_DEV,
                                                               KEY_READ);

                                if (hk != INVALID_HANDLE_VALUE) {
                                    BYTE edid_data[1024];
                                    DWORD edid_data_size = sizeof(edid_data);
                                    if (RegQueryValueEx(hk, L"EDID", NULL, NULL, edid_data, &edid_data_size) == 0) {
                                        if (!parse_edid(edid_data, edid_data_size)) {
                                            for (DWORD i = 0; i < edid_data_size; ++i) {
                                                printf("%02X ", edid_data[i]);
                                            }
                                            printf("\n");
                                        }
                                    }
                                }

                                //     static char keybuf[256];
                                //     DWORD keybuf_size = sizeof(keybuf);
                                //     static char classbuf[64];
                                //     DWORD classbuf_size = sizeof(classbuf);
                                //     FILETIME last_write_time;

                                //     int reg_index = 0;
                                //     while (RegEnumKeyExA(hk, reg_index, 
                                //                          keybuf, &keybuf_size,
                                //                          NULL, 
                                //                          classbuf, &classbuf_size,
                                //                          &last_write_time) == 0)
                                //     {
                                //         printf("\t\t- %s:%s\n", classbuf, keybuf);

                                //         HKEY hks0;
                                //         if (RegOpenKeyA(hk, keybuf, &hks0) == 0) {

                                //             char buf[128] = {0};
                                //             int hks0i = 0;
                                //             while (RegEnumKeyA(hks0, hks0i, buf, 128) == 0) {
                                //                 printf("\t\t\t- %s\n", buf);
                                //                 hks0i++;
                                //             }

                                //             RegCloseKey(hks0);
                                //         }

                                //         keybuf_size = sizeof(keybuf);
                                //         classbuf_size = sizeof(classbuf);
                                //         reg_index++;
                                //     }
                                // } else {
                                //     printf("SetupDiOpenDevRegKey Failed: %d\n", GetLastError());
                                // }
                                RegCloseKey(hk);
                            } else {
                                printf("SetupDiEnumDeviceInfo Failed: %d\n", GetLastError());
                            }
                        }

                    } else {
                        printf("SetupDiGetDeviceInterfaceDetailW Failed: %d\n", GetLastError());
                    }

                    di_index++;
                }

            } else {
                printf("SetupDiGetClassDevsW(&guid_devinterface_monitor, NULL, NULL, DIGCF_DEVICEINTERFACE) Failed: %d\n", GetLastError());
            }
        }
        adapter_index++;
    }

    return 0;
}
