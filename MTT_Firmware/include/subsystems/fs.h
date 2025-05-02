/* filesystem */

#pragma once

#include "esp_err.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"

#define FS_ROOT                         "/data"

class FS {
public:
    static esp_err_t init(bool rw = false); // initialise the filesystem (read-only or read-writable) - normally we want read-only for normal boot, and read-write for config CLI
    static esp_err_t deinit(); // unmount filesystem

    static bool isInitialised(); // return whether the filesystem has been mounted

private:
    static const char* kTag; // for logging

    static wl_handle_t m_wlHandle;
    static bool m_initialised; // set to true after successful initialisation
    static bool m_rw; // set to true if init was called with rw = true
};