/* NVS initialisation */

#pragma once

#include "nvs_flash.h"

class NVS {
public:
    static esp_err_t init(); // initialise NVS
    static bool isInitialised(); // return whether NVS has been initialised
private:
    static bool m_initialised;
};