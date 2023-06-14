#pragma once

#include <ArduinoBLE.h>

const char* BASE_UUID           = "00000000-9a74-4b30-9361-4a16ec09930f";

// Service 1: TENG
const char* TENG_SUUID          = "00010000-9a74-4b30-9361-4a16ec09930f";  // Service UUID
const char* TENG_STATUS_CUUID   = "00010001-9a74-4b30-9361-4a16ec09930f";  // Characteristic UUID
const char* TENG_COMMAND_CUUID  = "00010002-9a74-4b30-9361-4a16ec09930f";  // Characteristic UUID
const char* TENG_READING_CUUID  = "00010003-9a74-4b30-9361-4a16ec09930f";  // Characteristic UUID
BLEService tengService(TENG_SUUID);  
BLEByteCharacteristic tengStatus(TENG_STATUS_CUUID, BLERead | BLENotify);
BLEByteCharacteristic tengCommand(TENG_COMMAND_CUUID, BLEWrite);
BLEUnsignedShortCharacteristic tengReading(TENG_READING_CUUID, BLERead);

enum Command : byte { NOOP = 0, MEASURE_BASELINE }
