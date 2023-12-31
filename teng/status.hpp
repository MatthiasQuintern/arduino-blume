#pragma once

#include <Arduino.h>

#include "settings.hpp"
#include "services.hpp"

// needs to be in separate file because sketch preprocessor reorders stuff
enum DeviceStatus : byte { ERROR = 0, BUSY, WAIT_CONNECT, CONNECTED, MEASURING };

void setStatusLED(DeviceStatus s, bool value) {
    switch (s) {
        case ERROR:
            digitalWrite(LED_RED, value);
            break;
        case BUSY:
            digitalWrite(LED_YELLOW, value);
            digitalWrite(LED_RED, value);
            break;
        case WAIT_CONNECT:
            digitalWrite(LED_YELLOW, value);
            break;
        case CONNECTED:
            digitalWrite(LED_GREEN, value);
            break;
        /* case MEASURING_BASELINE: */
        /*     digitalWrite(LED_RED, value); */
        /*     digitalWrite(LED_GREEN, value); */
        /*     break; */
        case MEASURING:
            digitalWrite(LED_YELLOW, value);
            digitalWrite(LED_GREEN, value);
            break;
    }
}

DeviceStatus deviceStatus = WAIT_CONNECT;
void setStatus(DeviceStatus s) {
    setStatusLED(deviceStatus, LOW);
    setStatusLED(s, HIGH);
    deviceStatus = s;
    tengStatus.writeValue(deviceStatus);
}
