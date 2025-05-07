/* NTP client and time synchronisation */

#pragma once

#include "esp_err.h"

#include <sys/time.h>

class NTP {
public:
    static esp_err_t init(const char* server); // initialise and synchronise time using the given NTP server

private:
    static const char* kTag;

    static void syncCallback(timeval* tv); // callback for when time has been synchronised
};