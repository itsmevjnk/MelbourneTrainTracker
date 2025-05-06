/* message parsing */

#pragma once

#include <bits/stdc++.h>

#include <sys/time.h>
#include "colours.h"
#include "lsid.h"

#include <freertos/FreeRTOS.h>

#include "subsystems/led_matrix.h"

class Message {
public:
    static void parseMessage(const char* buffer, size_t bufferLength); // parse incoming message
private:
    static const char* kTag; // for logging
};