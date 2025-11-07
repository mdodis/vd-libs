#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "Advapi32.lib")

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
        if (EnumDisplayDevicesW(display_adapter.DeviceName, 0, &display_monitor, EDD_GET_DEVICE_INTERFACE_NAME)) {
            printf("%S\n", display_monitor.DeviceID);

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
                            printf("\t- %S\n", pdi_detail_data->DevicePath);

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
                                        for (DWORD i = 0; i < edid_data_size; ++i) {
                                            printf("%02X ", edid_data[i]);
                                        }
                                        printf("\n");
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
