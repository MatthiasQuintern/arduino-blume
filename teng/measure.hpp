#pragma once

#include <ArduinoBLE.h>

#include "services.hpp"
#include "status.hpp"

enum MeasurementTask { STOP_MEASURE, START_MEASURE, START_MEASURE_COUNT };
MeasurementTask measurementTask = STOP_MEASURE;

void measure(BLEDevice central, bool limitToCount=false) {
    uint16_t interval = tengInterval.value();
    uint16_t count = tengCount.value();

    Serial.print("Starting measurement with interval=");
    Serial.print(interval, DEC);
    if (limitToCount) {
        Serial.print(", count=");
        Serial.print(count, DEC);
    }
    Serial.println("");

    uint16_t i = 0;
    setStatus(DeviceStatus::MEASURING);
    while (central.connected() && (!limitToCount || i < count) && measurementTask != STOP_MEASURE) {
        uint16_t val = static_cast<uint16_t>(analogRead(PIN_TENG));
        Serial.println(val, DEC);
        tengReading.writeValue(val);
        /* Serial.println(val, DEC); */
        delay(interval);
        i++;
        central.poll();
    }
    measurementTask = STOP_MEASURE;
    setStatus(DeviceStatus::CONNECTED);
}
