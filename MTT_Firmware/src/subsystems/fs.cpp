#include "subsystems/fs.h"

#include "esp_log.h"
#include "esp_check.h"
const char* FS::kTag = "fs";

wl_handle_t FS::m_wlHandle;
bool FS::m_rw = false;
esp_err_t FS::init(bool rw) {
    if (m_initialised) {
        /* already mounted - unmount */
        esp_err_t ret = (m_rw) ? esp_vfs_fat_spiflash_unmount_rw_wl(FS_ROOT, m_wlHandle) : esp_vfs_fat_spiflash_unmount_ro(FS_ROOT, "storage");
        ESP_RETURN_ON_ERROR(ret, kTag, "unmounting failed (%s)", esp_err_to_name(ret));
    }

    m_initialised = false;
    
    esp_vfs_fat_mount_config_t config = {
        .format_if_mount_failed = true, // auto format if mounting fails
        .max_files = 4, // max number of open files
        .allocation_unit_size = 0 // auto select allocation unit size
    };
    esp_err_t ret = (rw) ? esp_vfs_fat_spiflash_mount_rw_wl(FS_ROOT, "storage", &config, &m_wlHandle) : esp_vfs_fat_spiflash_mount_ro(FS_ROOT, "storage", &config);
    ESP_RETURN_ON_ERROR(ret, kTag, "mounting failed (%s)", esp_err_to_name(ret));

    m_initialised = true; m_rw = rw;
    ESP_LOGI(kTag, "mounted internal flash, rw=%d", (rw) ? 1 : 0);
    return ESP_OK;
}

esp_err_t FS::deinit() {
    if (!m_initialised) {
        ESP_LOGW(kTag, "file system has not been initialised yet");
        return ESP_OK;
    }

    m_initialised = false;
    esp_err_t ret = (m_rw) ? esp_vfs_fat_spiflash_unmount_rw_wl(FS_ROOT, m_wlHandle) : esp_vfs_fat_spiflash_unmount_ro(FS_ROOT, "storage");
    ESP_RETURN_ON_ERROR(ret, kTag, "unmounting failed (%s)", esp_err_to_name(ret));

    ESP_LOGI(kTag, "file system unmounted");
    return ESP_OK;
}

bool FS::m_initialised = false;
bool FS::isInitialised() {
    return m_initialised;
}
