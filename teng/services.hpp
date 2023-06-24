#pragma once

#include <ArduinoBLE.h>

const char* BASE_UUID           = "00000000-9a74-4b30-9361-4a16ec09930f";

// Service 1: TENG
const char* TENG_SUUID          = "00010000-9a74-4b30-9361-4a16ec09930f";  // Service UUID
const char* TENG_STATUS_CUUID   = "00010001-9a74-4b30-9361-4a16ec09930f";  // Characteristic UUID
const char* TENG_COMMAND_CUUID  = "00010002-9a74-4b30-9361-4a16ec09930f";
const char* TENG_READING_CUUID  = "00010003-9a74-4b30-9361-4a16ec09930f";
const char* TENG_COUNT_CUUID    = "00010004-9a74-4b30-9361-4a16ec09930f";
const char* TENG_INTERVAL_CUUID = "00010005-9a74-4b30-9361-4a16ec09930f";
/* const char* TENG_BASELINE_CUUID = "00010004-9a74-4b30-9361-4a16ec09930f";  // Characteristic UUID */
BLEService tengService(TENG_SUUID);  
BLEByteCharacteristic tengStatus(TENG_STATUS_CUUID, BLERead | BLENotify);
BLEByteCharacteristic tengCommand(TENG_COMMAND_CUUID, BLEWrite);
BLEUnsignedShortCharacteristic tengReading(TENG_READING_CUUID, BLERead | BLENotify);
BLEUnsignedShortCharacteristic tengInterval(TENG_INTERVAL_CUUID, BLERead | BLEWrite);
BLEUnsignedShortCharacteristic tengCount(TENG_COUNT_CUUID, BLERead | BLEWrite);
/* BLEUnsignedShortCharacteristic tengBaseline(TENG_BASELINE_CUUID, BLERead); */

enum Command : byte { STOP = 0, MEASURE_COUNT, MEASURE, };

void initServices() {
    tengService.addCharacteristic(tengStatus);
    tengService.addCharacteristic(tengCommand);
    tengService.addCharacteristic(tengReading);
    tengService.addCharacteristic(tengCount);
    tengService.addCharacteristic(tengInterval);
    /* tengService.addCharacteristic(tengBaseline); */
}
