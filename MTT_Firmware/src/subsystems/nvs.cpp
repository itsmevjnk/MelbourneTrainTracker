#include "subsystems/nvs.h"

/* for logging */
#include "esp_log.h"
const char* NVS::kTag = "nvs";

#include "esp_check.h"

bool NVS::m_initialised = false;
bool NVS::isInitialised() {
    return m_initialised;
}

esp_err_t NVS::init() {
    if (m_initialised) {
        ESP_LOGW(kTag, "NVS has already been initialised");
        return ESP_OK;
    }

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGI(kTag, "erasing NVS");
        ESP_RETURN_ON_ERROR(nvs_flash_erase(), kTag, "NVS erase failed");
        ret = nvs_flash_init();
    }

    ESP_RETURN_ON_ERROR(ret, kTag, "NVS initialisation failed");

    m_initialised = true;
    ESP_LOGI(kTag, "NVS initialised successfully");
    return ESP_OK;
}

NVSHandle NVS::open(const char* name, nvs_open_mode_t mode) {
    nvs_handle_t handle;

    esp_err_t ret = nvs_open(name, mode, &handle);
    if (ret != ESP_OK) {
        ESP_LOGE(kTag, "cannot open NVS namespace %s (%s)", name, esp_err_to_name(ret));
        return NVSHandle();
    }

    return NVSHandle(handle, (mode == NVS_READWRITE));
}

const char* NVSHandle::kTag = "nvs:handle";

bool NVSHandle::isClosed() {
    return m_closed;
}

esp_err_t NVSHandle::close() {
    if (m_closed) {
        ESP_LOGW(kTag, "handle %lu has already been closed", m_handle);
        return ESP_OK;
    }

    if (m_rw) {
        ESP_RETURN_ON_ERROR(nvs_commit(m_handle), kTag, "cannot commit handle %lu", m_handle);
    }

    nvs_close(m_handle);
    ESP_LOGV(kTag, "handle %lu closed", m_handle);
    m_closed = true;
    return ESP_OK;
}

esp_err_t NVSHandle::getU8(const char* key, uint8_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_u8(m_handle, key, value);
}

esp_err_t NVSHandle::getS8(const char* key, int8_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_i8(m_handle, key, value);
}

esp_err_t NVSHandle::getU16(const char* key, uint16_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_u16(m_handle, key, value);
}

esp_err_t NVSHandle::getS16(const char* key, int16_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_i16(m_handle, key, value);
}

esp_err_t NVSHandle::getU32(const char* key, uint32_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_u32(m_handle, key, value);
}

esp_err_t NVSHandle::getS32(const char* key, int32_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_i32(m_handle, key, value);
}

esp_err_t NVSHandle::getU64(const char* key, uint64_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_u64(m_handle, key, value);
}

esp_err_t NVSHandle::getS64(const char* key, int64_t* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_i64(m_handle, key, value);
}

esp_err_t NVSHandle::getString(const char* key, char* value, size_t maxLength, size_t* length) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    ESP_RETURN_ON_ERROR(nvs_get_str(m_handle, key, value, &maxLength), kTag, "cannot read string %s of handle %lu", key, m_handle);
    if (length) *length = maxLength;
    return ESP_OK;    
}

esp_err_t NVSHandle::getBlob(const char* key, void* value, size_t maxLength, size_t* length) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    ESP_RETURN_ON_ERROR(nvs_get_blob(m_handle, key, value, &maxLength), kTag, "cannot read blob %s of handle %lu", key, m_handle);
    if (length) *length = maxLength;
    return ESP_OK;    
}

esp_err_t NVSHandle::getStringLength(const char* key, size_t* length) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_str(m_handle, key, nullptr, length);
}

esp_err_t NVSHandle::getBlobLength(const char* key, size_t* length) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_get_blob(m_handle, key, nullptr, length);
}

esp_err_t NVSHandle::setU8(const char* key, uint8_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_u8(m_handle, key, value);
}

esp_err_t NVSHandle::setS8(const char* key, int8_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_i8(m_handle, key, value);
}

esp_err_t NVSHandle::setU16(const char* key, uint16_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_u16(m_handle, key, value);
}

esp_err_t NVSHandle::setS16(const char* key, int16_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_i16(m_handle, key, value);
}

esp_err_t NVSHandle::setU32(const char* key, uint32_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_u32(m_handle, key, value);
}

esp_err_t NVSHandle::setS32(const char* key, int32_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_i32(m_handle, key, value);
}

esp_err_t NVSHandle::setU64(const char* key, uint64_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_u64(m_handle, key, value);
}

esp_err_t NVSHandle::setS64(const char* key, int64_t value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_i64(m_handle, key, value);
}

esp_err_t NVSHandle::setString(const char* key, const char* value) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_str(m_handle, key, value);
}

esp_err_t NVSHandle::setBlob(const char* key, const void* value, size_t length) {
    ESP_RETURN_ON_FALSE(!m_closed, ESP_ERR_INVALID_STATE, kTag, "handle %lu is already closed", m_handle);
    return nvs_set_blob(m_handle, key, value, length);
}
