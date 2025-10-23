#include <windows.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <hidclass.h>
#include <stdio.h>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

static HANDLE           _Process_Heap;
static HIDP_BUTTON_CAPS _Button_Caps_Buffer[128];
static HIDP_VALUE_CAPS  _Value_Caps_Buffer[128];

static const char *usb_usage_page_name(USHORT usage_page);

int main(int argc, char *argv[])
{
    _Process_Heap = GetProcessHeap();

    GUID hid_guid;
    HidD_GetHidGuid(&hid_guid);

    HDEVINFO device_info_set = SetupDiGetClassDevs(&hid_guid,
                                                   NULL,
                                                   NULL,
                                                   DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    SP_DEVICE_INTERFACE_DATA device_interface_data;
    device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    DWORD device_index = 0;

    while (SetupDiEnumDeviceInterfaces(device_info_set,
                                       NULL,
                                       &hid_guid,
                                       device_index,
                                       &device_interface_data))
    {
        device_index++;

        DWORD required_size = 0;
        SetupDiGetDeviceInterfaceDetail(device_info_set, &device_interface_data, NULL, 0, &required_size, NULL);

        void *pdetail = (void*)HeapAlloc(_Process_Heap, 0, (SIZE_T)required_size);
        PSP_DEVICE_INTERFACE_DETAIL_DATA detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)pdetail;
        detail_data->cbSize = sizeof(*detail_data);

        if (!SetupDiGetDeviceInterfaceDetail(device_info_set, &device_interface_data, detail_data, required_size, NULL, NULL))
        {
            fprintf(stderr, "[%d] SetupDiGetDeviceInterfaceDetail Failed: %d\n", device_index, GetLastError());
            continue;
        }

        printf("%s\n", detail_data->DevicePath);

        HANDLE device_handle = CreateFileA(detail_data->DevicePath,
                                           GENERIC_READ | GENERIC_WRITE,
                                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                                           NULL,
                                           OPEN_EXISTING,
                                           FILE_FLAG_OVERLAPPED,
                                           NULL);
        if (device_handle == INVALID_HANDLE_VALUE) {
            printf("\tN/A\n");
            continue;
        }

        {
            wchar_t temp_wstring[128];
            if (HidD_GetManufacturerString(device_handle, temp_wstring, sizeof(temp_wstring))) {
                wprintf(L"\tManufacturer: %s\n", temp_wstring);
            }

            if (HidD_GetProductString(device_handle, temp_wstring, sizeof(temp_wstring))) {
                wprintf(L"\tProduct: %s\n", temp_wstring);
            }
        }

        HIDD_ATTRIBUTES attributes;
        attributes.Size = sizeof(attributes);
        if (HidD_GetAttributes(device_handle, &attributes))
        {
            printf("\tVID: 0x%04x\n", attributes.VendorID);
            printf("\tPID: 0x%04x\n", attributes.ProductID);
            printf("\tVER: 0x%04x\n", attributes.VersionNumber);
        }

        PHIDP_PREPARSED_DATA preparsed_data = NULL;
        if (!HidD_GetPreparsedData(device_handle, &preparsed_data))
        {
            fprintf(stderr, "[%d] HidD_GetPreparsedData Failed: %d\n", device_index, GetLastError());
            goto END_CLOSE;
        }

        HIDP_CAPS caps;
        if (HidP_GetCaps(preparsed_data, &caps) != HIDP_STATUS_SUCCESS) {
            fprintf(stderr, "[%d] HidP_GetCaps Failed: %d", device_index, GetLastError());
            goto END_CLOSE;
        }

        printf("\tUsage:                     0x%04x\n",      caps.Usage);
        printf("\tUsagePage:                 0x%04x (%s)\n", caps.UsagePage, usb_usage_page_name(caps.UsagePage));
        printf("\tInputReportByteLength:     %-4d\n",        caps.InputReportByteLength);
        printf("\tOutputReportByteLength:    %-4d\n",        caps.OutputReportByteLength);
        printf("\tFeatureReportByteLength:   %-4d\n",        caps.FeatureReportByteLength);
        printf("\tNumberLinkCollectionNodes: %-4d\n",        caps.NumberLinkCollectionNodes);
        printf("\tNumberInputButtonCaps:     %-4d\n",        caps.NumberInputButtonCaps);
        {
            USHORT len = sizeof(_Button_Caps_Buffer) / sizeof(_Button_Caps_Buffer[0]);

            if (HidP_GetButtonCaps(HidP_Input,
                                   _Button_Caps_Buffer,
                                   &len,
                                   preparsed_data) != HIDP_STATUS_SUCCESS)
            {
                fprintf(stderr, "[%d] HidP_GetButtonCaps for HidP_Input Failed: %d\n", device_index, GetLastError());
                goto AFTER_INPUT_BUTTON_CAPS;
            }

            for (USHORT i = 0; i < len; ++i) {
                HIDP_BUTTON_CAPS *cap = &_Button_Caps_Buffer[i];

                printf("\t\t[%u] Range: %d\n", i, cap->IsRange);
                printf("\t\t\tUsagePage:                0x%04x (%s)\n", /*USAGE*/   cap->UsagePage, usb_usage_page_name(cap->UsagePage));
                printf("\t\t\tReportID:                 0x%04x\n",      /*UCHAR*/   cap->ReportID);
                printf("\t\t\tIsAlias:                  %-4d\n",        /*BOOLEAN*/ cap->IsAlias);
                printf("\t\t\tBitField:                 0x%04x\n",      /*USHORT*/  cap->BitField);
                printf("\t\t\tLinkCollection:           %-4d\n",        /*USHORT*/  cap->LinkCollection);
                printf("\t\t\tLinkUsage:                0x%04x\n",      /*USAGE*/   cap->LinkUsage);
                printf("\t\t\tLinkUsagePage:            0x%04x\n",      /*USAGE*/   cap->LinkUsagePage);
                printf("\t\t\tIsStringRange:            %-4d\n",        /*BOOLEAN*/ cap->IsStringRange);
                printf("\t\t\tIsDesignatorRange:        %-4d\n",        /*BOOLEAN*/ cap->IsDesignatorRange);
                printf("\t\t\tIsAbsolute:               %-4d\n",        /*BOOLEAN*/ cap->IsAbsolute);
                printf("\t\t\tReportCount:              %-4d\n",        /*USHORT*/  cap->ReportCount);
                if (!cap->IsRange) {
                    printf("\t\t\tNotRange.Usage:           0x%04x\n", /*USAGE*/  cap->NotRange.Usage);
                    printf("\t\t\tNotRange.StringIndex:     %-4d\n",   /*USHORT*/ cap->NotRange.StringIndex);
                    printf("\t\t\tNotRange.DesignatorIndex: %-4d\n",   /*USHORT*/ cap->NotRange.DesignatorIndex);
                    printf("\t\t\tNotRange.DataIndex:       %-4d\n",   /*USHORT*/ cap->NotRange.DataIndex);
                } else {
                    printf("\t\t\tRange.UsageMin:           0x%04x\n", /*USAGE*/  cap->Range.UsageMin);
                    printf("\t\t\tRange.UsageMax:           0x%04x\n", /*USAGE*/  cap->Range.UsageMax);
                    printf("\t\t\tRange.StringMin:          %-4d\n",   /*USHORT*/ cap->Range.StringMin);
                    printf("\t\t\tRange.StringMax:          %-4d\n",   /*USHORT*/ cap->Range.StringMax);
                    printf("\t\t\tRange.DesignatorMin:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMin);
                    printf("\t\t\tRange.DesignatorMax:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMax);
                    printf("\t\t\tRange.DataIndexMin:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMin);
                    printf("\t\t\tRange.DataIndexMax:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMax);
                }
            }
        }
AFTER_INPUT_BUTTON_CAPS:
        printf("\tNumberInputValueCaps:      %-4d\n", caps.NumberInputValueCaps);
        {
            USHORT len = sizeof(_Value_Caps_Buffer) / sizeof(_Value_Caps_Buffer[0]);
            if (HidP_GetValueCaps(HidP_Input,
                                  _Value_Caps_Buffer,
                                  &len,
                                  preparsed_data) != HIDP_STATUS_SUCCESS)
            {

                fprintf(stderr, "[%d] HidP_GetValueCaps for HidP_Input Failed: %d\n", device_index, GetLastError());
                goto AFTER_INPUT_VALUE_CAPS;
            }

            for (USHORT i = 0; i < len; ++i) {
                HIDP_VALUE_CAPS *cap = &_Value_Caps_Buffer[i];
                printf("\t\t[%u] Range: %d\n", i, cap->IsRange);
                printf("\t\t\tUsagePage:                0x%04x (%s)\n", /*USAGE*/   cap->UsagePage, usb_usage_page_name(cap->UsagePage));
                printf("\t\t\tReportID:                 %-4d\n",        /*UCHAR*/   cap->ReportID);
                printf("\t\t\tIsAlias:                  %-4d\n",        /*BOOLEAN*/ cap->IsAlias);
                printf("\t\t\tBitField:                 %-4d\n",        /*USHORT*/  cap->BitField);
                printf("\t\t\tLinkCollection:           %-4d\n",        /*USHORT*/  cap->LinkCollection);
                printf("\t\t\tLinkUsage:                %04x\n",        /*USAGE*/   cap->LinkUsage);
                printf("\t\t\tLinkUsagePage:            %04x\n",        /*USAGE*/   cap->LinkUsagePage);
                printf("\t\t\tIsRange:                  %-4d\n",        /*BOOLEAN*/ cap->IsRange);
                printf("\t\t\tIsStringRange:            %-4d\n",        /*BOOLEAN*/ cap->IsStringRange);
                printf("\t\t\tIsDesignatorRange:        %-4d\n",        /*BOOLEAN*/ cap->IsDesignatorRange);
                printf("\t\t\tIsAbsolute:               %-4d\n",        /*BOOLEAN*/ cap->IsAbsolute);
                printf("\t\t\tHasNull:                  %-4d\n",        /*BOOLEAN*/ cap->HasNull);
                printf("\t\t\tBitSize:                  %-4d\n",        /*USHORT*/  cap->BitSize);
                printf("\t\t\tReportCount:              %-4d\n",        /*USHORT*/  cap->ReportCount);
                printf("\t\t\tUnitsExp:                 %-4d\n",        /*ULONG*/   cap->UnitsExp);
                printf("\t\t\tUnits:                    %-4d\n",        /*ULONG*/   cap->Units);
                printf("\t\t\tLogicalMin:               %-8d\n",        /*LONG*/    cap->LogicalMin);
                printf("\t\t\tLogicalMax:               %-8d\n",        /*LONG*/    cap->LogicalMax);
                printf("\t\t\tPhysicalMin:              %-8d\n",        /*LONG*/    cap->PhysicalMin);
                printf("\t\t\tPhysicalMax:              %-8d\n",        /*LONG*/    cap->PhysicalMax);
                if (!cap->IsRange) {
                    printf("\t\t\tNotRange.Usage:           0x%04x\n", /*USAGE*/  cap->NotRange.Usage);
                    printf("\t\t\tNotRange.StringIndex:     %-4d\n",   /*USHORT*/ cap->NotRange.StringIndex);
                    printf("\t\t\tNotRange.DataIndex:       %-4d\n",   /*USHORT*/ cap->NotRange.DataIndex);
                } else {
                    printf("\t\t\tRange.UsageMin:           0x%04x\n", /*USAGE*/  cap->Range.UsageMin);
                    printf("\t\t\tRange.UsageMax:           0x%04x\n", /*USAGE*/  cap->Range.UsageMax);
                    printf("\t\t\tRange.StringMin:          %-4d\n",   /*USHORT*/ cap->Range.StringMin);
                    printf("\t\t\tRange.StringMax:          %-4d\n",   /*USHORT*/ cap->Range.StringMax);
                    printf("\t\t\tRange.DesignatorMin:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMin);
                    printf("\t\t\tRange.DesignatorMax:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMax);
                    printf("\t\t\tRange.DataIndexMin:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMin);
                    printf("\t\t\tRange.DataIndexMax:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMax);
                }
            }
        }
AFTER_INPUT_VALUE_CAPS:
        printf("\tNumberInputDataIndices:    %-4d\n", caps.NumberInputDataIndices);
        printf("\tNumberOutputButtonCaps:    %-4d\n", caps.NumberOutputButtonCaps);
        {
            USHORT len = sizeof(_Button_Caps_Buffer) / sizeof(_Button_Caps_Buffer[0]);

            if (HidP_GetButtonCaps(HidP_Input,
                                   _Button_Caps_Buffer,
                                   &len,
                                   preparsed_data) != HIDP_STATUS_SUCCESS)
            {
                fprintf(stderr, "[%d] HidP_GetButtonCaps for HidP_Output Failed: %d\n", device_index, GetLastError());
                goto AFTER_OUTPUT_BUTTON_CAPS;
            }

            for (USHORT i = 0; i < len; ++i) {
                HIDP_BUTTON_CAPS *cap = &_Button_Caps_Buffer[i];

                printf("\t\t[%u] Range: %d\n", i, cap->IsRange);
                printf("\t\t\tUsagePage:                0x%04x (%s)\n", /*USAGE*/   cap->UsagePage, usb_usage_page_name(cap->UsagePage));
                printf("\t\t\tReportID:                 0x%04x\n",      /*UCHAR*/   cap->ReportID);
                printf("\t\t\tIsAlias:                  %-4d\n",        /*BOOLEAN*/ cap->IsAlias);
                printf("\t\t\tBitField:                 0x%04x\n",      /*USHORT*/  cap->BitField);
                printf("\t\t\tLinkCollection:           %-4d\n",        /*USHORT*/  cap->LinkCollection);
                printf("\t\t\tLinkUsage:                0x%04x\n",      /*USAGE*/   cap->LinkUsage);
                printf("\t\t\tLinkUsagePage:            0x%04x\n",      /*USAGE*/   cap->LinkUsagePage);
                printf("\t\t\tIsStringRange:            %-4d\n",        /*BOOLEAN*/ cap->IsStringRange);
                printf("\t\t\tIsDesignatorRange:        %-4d\n",        /*BOOLEAN*/ cap->IsDesignatorRange);
                printf("\t\t\tIsAbsolute:               %-4d\n",        /*BOOLEAN*/ cap->IsAbsolute);
                printf("\t\t\tReportCount:              %-4d\n",        /*USHORT*/  cap->ReportCount);
                if (!cap->IsRange) {
                    printf("\t\t\tNotRange.Usage:           0x%04x\n", /*USAGE*/  cap->NotRange.Usage);
                    printf("\t\t\tNotRange.StringIndex:     %-4d\n",   /*USHORT*/ cap->NotRange.StringIndex);
                    printf("\t\t\tNotRange.DesignatorIndex: %-4d\n",   /*USHORT*/ cap->NotRange.DesignatorIndex);
                    printf("\t\t\tNotRange.DataIndex:       %-4d\n",   /*USHORT*/ cap->NotRange.DataIndex);
                } else {
                    printf("\t\t\tRange.UsageMin:           0x%04x\n", /*USAGE*/  cap->Range.UsageMin);
                    printf("\t\t\tRange.UsageMax:           0x%04x\n", /*USAGE*/  cap->Range.UsageMax);
                    printf("\t\t\tRange.StringMin:          %-4d\n",   /*USHORT*/ cap->Range.StringMin);
                    printf("\t\t\tRange.StringMax:          %-4d\n",   /*USHORT*/ cap->Range.StringMax);
                    printf("\t\t\tRange.DesignatorMin:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMin);
                    printf("\t\t\tRange.DesignatorMax:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMax);
                    printf("\t\t\tRange.DataIndexMin:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMin);
                    printf("\t\t\tRange.DataIndexMax:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMax);
                }
            }
        }
AFTER_OUTPUT_BUTTON_CAPS:

        printf("\tNumberOutputValueCaps:     %-4d\n", caps.NumberOutputValueCaps);
        {
            USHORT len = sizeof(_Value_Caps_Buffer) / sizeof(_Value_Caps_Buffer[0]);
            if (HidP_GetValueCaps(HidP_Output,
                                  _Value_Caps_Buffer,
                                  &len,
                                  preparsed_data) != HIDP_STATUS_SUCCESS)
            {

                fprintf(stderr, "[%d] HidP_GetValueCaps for HidP_Output Failed: %d\n", device_index, GetLastError());
                goto AFTER_OUTPUT_VALUE_CAPS;
            }

            for (USHORT i = 0; i < len; ++i) {
                HIDP_VALUE_CAPS *cap = &_Value_Caps_Buffer[i];
                printf("\t\t[%u] Range: %d\n", i, cap->IsRange);
                printf("\t\t\tUsagePage:                0x%04x (%s)\n", /*USAGE*/   cap->UsagePage, usb_usage_page_name(cap->UsagePage));
                printf("\t\t\tReportID:                 %-4d\n",        /*UCHAR*/   cap->ReportID);
                printf("\t\t\tIsAlias:                  %-4d\n",        /*BOOLEAN*/ cap->IsAlias);
                printf("\t\t\tBitField:                 %-4d\n",        /*USHORT*/  cap->BitField);
                printf("\t\t\tLinkCollection:           %-4d\n",        /*USHORT*/  cap->LinkCollection);
                printf("\t\t\tLinkUsage:                %04x\n",        /*USAGE*/   cap->LinkUsage);
                printf("\t\t\tLinkUsagePage:            %04x\n",        /*USAGE*/   cap->LinkUsagePage);
                printf("\t\t\tIsRange:                  %-4d\n",        /*BOOLEAN*/ cap->IsRange);
                printf("\t\t\tIsStringRange:            %-4d\n",        /*BOOLEAN*/ cap->IsStringRange);
                printf("\t\t\tIsDesignatorRange:        %-4d\n",        /*BOOLEAN*/ cap->IsDesignatorRange);
                printf("\t\t\tIsAbsolute:               %-4d\n",        /*BOOLEAN*/ cap->IsAbsolute);
                printf("\t\t\tHasNull:                  %-4d\n",        /*BOOLEAN*/ cap->HasNull);
                printf("\t\t\tBitSize:                  %-4d\n",        /*USHORT*/  cap->BitSize);
                printf("\t\t\tReportCount:              %-4d\n",        /*USHORT*/  cap->ReportCount);
                printf("\t\t\tUnitsExp:                 %-4d\n",        /*ULONG*/   cap->UnitsExp);
                printf("\t\t\tUnits:                    %-4d\n",        /*ULONG*/   cap->Units);
                printf("\t\t\tLogicalMin:               %-8d\n",        /*LONG*/    cap->LogicalMin);
                printf("\t\t\tLogicalMax:               %-8d\n",        /*LONG*/    cap->LogicalMax);
                printf("\t\t\tPhysicalMin:              %-8d\n",        /*LONG*/    cap->PhysicalMin);
                printf("\t\t\tPhysicalMax:              %-8d\n",        /*LONG*/    cap->PhysicalMax);
                if (!cap->IsRange) {
                    printf("\t\t\tNotRange.Usage:           0x%04x\n", /*USAGE*/  cap->NotRange.Usage);
                    printf("\t\t\tNotRange.StringIndex:     %-4d\n",   /*USHORT*/ cap->NotRange.StringIndex);
                    printf("\t\t\tNotRange.DataIndex:       %-4d\n",   /*USHORT*/ cap->NotRange.DataIndex);
                } else {
                    printf("\t\t\tRange.UsageMin:           0x%04x\n", /*USAGE*/  cap->Range.UsageMin);
                    printf("\t\t\tRange.UsageMax:           0x%04x\n", /*USAGE*/  cap->Range.UsageMax);
                    printf("\t\t\tRange.StringMin:          %-4d\n",   /*USHORT*/ cap->Range.StringMin);
                    printf("\t\t\tRange.StringMax:          %-4d\n",   /*USHORT*/ cap->Range.StringMax);
                    printf("\t\t\tRange.DesignatorMin:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMin);
                    printf("\t\t\tRange.DesignatorMax:      %-4d\n",   /*USHORT*/ cap->Range.DesignatorMax);
                    printf("\t\t\tRange.DataIndexMin:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMin);
                    printf("\t\t\tRange.DataIndexMax:       %-4d\n",   /*USHORT*/ cap->Range.DataIndexMax);
                }
            }
        }
AFTER_OUTPUT_VALUE_CAPS:


        printf("\tNumberOutputDataIndices:   %-4d\n", caps.NumberOutputDataIndices);
        printf("\tNumberFeatureButtonCaps:   %-4d\n", caps.NumberFeatureButtonCaps);
        printf("\tNumberFeatureValueCaps:    %-4d\n", caps.NumberFeatureValueCaps);
        printf("\tNumberFeatureDataIndices:  %-4d\n", caps.NumberFeatureDataIndices);


END_CLOSE:
        CloseHandle(device_handle);
    }
}

static const char *usb_usage_page_name(USHORT usage_page)
{
    switch (usage_page) {
        case 0x0001: return "Generic Desktop";         break;
        case 0x0002: return "Simulation Controls";     break;
        case 0x0003: return "VR Controls";             break;
        case 0x0004: return "Sport Controls";          break;
        case 0x0005: return "Game Controls";           break;
        case 0x0006: return "Generic Device Controls"; break;
        case 0x0007: return "Keyboard/Keypad";         break;
        case 0x0008: return "LEDs";                    break;
        case 0x0009: return "Button";                  break;
        case 0x000A: return "Ordinal";                 break;
        case 0x000B: return "Telephony Device";        break;
        case 0x000C: return "Consumer";                break;
        case 0x000D: return "Digitizers";              break;
        case 0x0010: return "Unicode";                 break;
        case 0x0014: return "Alphanumeric Display";    break;
        case 0x0040: return "Medical Instrument";      break;
        default:     return "N/A";                     break;
    }
}