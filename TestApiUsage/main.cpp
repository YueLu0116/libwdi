#include "../libwdi/libwdi.h"

#include <cstdio>
#include <iostream>
#include <string>

const std::string DEFAULT_DIR = "tmp";
const std::string INF_NAME = "MI_MAX_3.inf";
const uint16_t targetVid = 0x2717;
const uint16_t targetPid = 0xFF60;

inline bool isTarget(uint16_t vid, uint16_t pid) {
    return vid == targetVid && pid == targetPid;
}

int main(int argc, char* argv[]) {

    struct wdi_device_info* device, * list;
    wdi_options_create_list optList{ true, false, false };
    wdi_options_prepare_driver optPrepare{ 0 };
  
    int ret = wdi_create_list(&list, &optList);
    if (ret == WDI_SUCCESS) {
        for (device = list; device != NULL; device = device->next) {
            printf("Installing driver for [USB device: \"%s\" (%04X:%04X) driver type=%s]\n",
                device->desc, device->vid, device->pid, device->driver);
            if (isTarget(static_cast<uint16_t>(device->vid), static_cast<uint16_t>(device->pid))) {
                printf("Find the target device! [device=%s]", device->desc);
                if (wdi_prepare_driver(device, DEFAULT_DIR.c_str(), INF_NAME.c_str(), &optPrepare) == WDI_SUCCESS) {
                    printf("Done for preparing driver.\n");
                    if (wdi_install_driver(device, DEFAULT_DIR.c_str(), INF_NAME.c_str(), NULL) == WDI_SUCCESS) {
                        printf("Done for installing driver.\n");
                    }
                }
                break;
            }
        }
    }
    else {
        printf("Failed to create list. [Reason=%d]\n", ret);
    }
    wdi_destroy_list(list);
    system("pause");
    return 0;
}