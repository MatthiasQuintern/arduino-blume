#pragma once

#include <Arduino.h>

class Counter {
    public:
        const unsigned MAX_NUMBER = 4096;  // 12-bit
        const int RES = 18;
        const int CB  = 19;
        void reset() {
            digitalWrite(CB, HIGH);
            digitalWrite(RES, HIGH);
            delay(1);
            digitalWrite(RES, LOW);
            currentValue = 0;
        }
        /**
          * @brief Increment the counter value
          * @returns true if overflown to zero
          */
        bool increment() {
            digitalWrite(CB, LOW);
            delay(1);
            digitalWrite(CB, HIGH);
            currentValue++;
            if (currentValue >= MAX_NUMBER) {
                currentValue = 0;
                return true;
            }
            return false;
        }
        unsigned get() { return currentValue; }
    private:
        unsigned currentValue = 0;
};

class AddressState {
    public:
        AddressState(Counter counter_, int addressPinCount_, const int* addressPins_) 
          : counter(counter_), addressPinCount(addressPinCount_), addressPins(addressPins_) {
            counter.reset();
        }
        void set(unsigned targetAddress) { 
            unsigned targetCounterValue = targetAddress % counter.MAX_NUMBER; 
            if (counter.get() > targetCounterValue) {
                counter.reset();

            }
            for (unsigned i = 0; i < (targetCounterValue - counter.get()); i++) {
                counter.increment();
            }

            unsigned pinValues = targetAddress / counter.MAX_NUMBER; 
            addressPinStateMask = 0;
            for (unsigned i = 0; i < addressPinCount; i++) {
                unsigned bitSet = pinValues & (1 << i);
                if (bitSet > 0) {
                    digitalWrite(addressPins[i], HIGH);
                    addressPinStateMask |= (1 << (addressPins[i] - 1));
                }
                else {
                    digitalWrite(addressPins[i], LOW);

                }
            }
            address = targetAddress;
        }


        void increment() { 
            if (counter.increment()) {  // if overflow
                // add 1 to the other pins
                for (unsigned i = 0; i < addressPinCount; i++) {
                    if ((addressPinStateMask & (addressPins[i] - 1)) == 0) {
                        addressPinStateMask |= (1 << (addressPins[i] - 1));
                        digitalWrite(addressPins[i], HIGH);
                        break;
                    }
                    else {
                        addressPinStateMask ^= (1 << (addressPins[i] - 1));
                        digitalWrite(addressPins[i], LOW);
                    }
                }   
            }
            address++;
        }
        

        unsigned getAddress() { return address; }
        unsigned reset() {
            counter.reset();
            for (unsigned i = 0; i < addressPinCount; i++) {
                digitalWrite(addressPins[i], LOW);
            }
        }
    private:
        // if counter is 12 bit and address is 16 bit, this is the value of the top 4 bits
        // i-th bit represents i-th pin
        unsigned addressPinStateMask = 0;  
        unsigned address = 0;
        Counter counter;
        const int* addressPins;
        int addressPinCount;
};
