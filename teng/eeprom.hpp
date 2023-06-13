#pragma once

#include <Arduino.h>

#include "address.hpp"

class EEPROM {
    public:
        void setup() {
            for (unsigned i = 0; i < addressPinCount; i++) {
                pinMode(addressPins[i], OUTPUT);
            }
            for (unsigned i = 0; i < 8; i++) {
                pinMode(dataPins[i], INPUT);
            }
            pinMode(OEb, OUTPUT);
            pinMode(WEb, OUTPUT);
        }

        /**
         * @brief Read the data pins.
         */
        uint8_t readDataPins() {
            uint8_t b = 0;
            for (unsigned i = 0; i < 8; i++) {
                b |= (digitalRead(dataPins[i]) << i);
            }
            return b;
        }

        // set start address in via AddressState
        void read(uint8_t* data, unsigned dataSize, AddressState& address) {
            unsigned i = 0;
            for (unsigned i = 0; i < dataSize; i++) {
                digitalWrite(OEb, LOW);
                delay(t_OUTPUT_DELAY);
                data[i] = readDataPins();
                digitalWrite(OEb, HIGH);
                delay(t_OUTPUT_FLOAT);
                address.increment();
            }
        }
        
    /* private: */
        int OEb;
        int WEb;
        int addressPins[6];
        unsigned addressPinCount;
        int dataPins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

        // in ms
        unsigned t_OUTPUT_DELAY = 1;
        unsigned t_MIN_WRITE_PULSE_WIDTH = 1;
        unsigned t_OUTPUT_FLOAT = 1;
        unsigned t_DATA_HOLD_TIME = 0;
};

constexpr EEPROM AT28C256 {
    .OEb = 17,
    .WEb = 16,
    .addressPins = {10, 15, 11},
    .addressPinCount = 3
};

constexpr EEPROM SST39SF02A {
    .OEb = 17,
    .WEb = 16,
    .addressPins = {10, 15, 16, 11, 12, 14},
    .addressPinCount = 6
};
