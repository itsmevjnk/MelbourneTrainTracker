/* NVS */

#pragma once

#include "nvs_flash.h"
#include "esp_log.h"


/* wrapper for NVS handles */
class NVSHandle {
public:
    NVSHandle() : m_handle(0), m_closed(true), m_rw(true) {} // dummy object
    
    NVSHandle(nvs_handle_t handle, bool rw) : m_handle(handle), m_rw(rw) {}
    ~NVSHandle() { close(); }

    bool isClosed();

    esp_err_t close();

    esp_err_t getU8(const char* key, uint8_t* value);
    esp_err_t getS8(const char* key, int8_t* value);
    esp_err_t getU16(const char* key, uint16_t* value);
    esp_err_t getS16(const char* key, int16_t* value);
    esp_err_t getU32(const char* key, uint32_t* value);
    esp_err_t getS32(const char* key, int32_t* value);
    esp_err_t getU64(const char* key, uint64_t* value);
    esp_err_t getS64(const char* key, int64_t* value);
    esp_err_t getString(const char* key, char* value, size_t maxLength, size_t* length = nullptr);
    esp_err_t getBlob(const char* key, void* value, size_t maxLength, size_t* length = nullptr);

    esp_err_t getStringLength(const char* key, size_t* length);
    esp_err_t getBlobLength(const char* key, size_t* length);

    esp_err_t setU8(const char* key, uint8_t value);
    esp_err_t setS8(const char* key, int8_t value);
    esp_err_t setU16(const char* key, uint16_t value);
    esp_err_t setS16(const char* key, int16_t value);
    esp_err_t setU32(const char* key, uint32_t value);
    esp_err_t setS32(const char* key, int32_t value);
    esp_err_t setU64(const char* key, uint64_t value);
    esp_err_t setS64(const char* key, int64_t value);
    esp_err_t setString(const char* key, const char* value);
    esp_err_t setBlob(const char* key, const void* value, size_t length);

private:
    static const char* kTag;

    nvs_handle_t m_handle;
    bool m_closed = false;
    bool m_rw;
};

class NVS {
public:
    static esp_err_t init(); // initialise NVS
    static bool isInitialised(); // return whether NVS has been initialised

    static NVSHandle open(const char* name, nvs_open_mode_t mode);

private:
    static const char* kTag;
    static bool m_initialised;
};
