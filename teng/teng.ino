/*
 * 
 */

#include <Arduino.h>
#include <ArduinoBLE.h>

#include "settings.hpp"
#include "status.hpp"
#include "services.hpp"
#include "measure.hpp"

// TODO
/* std::array<uint16_t, 10000> valueBuffer;  // 20kB buffer for readings */
/* auto valueBufferWriteIter = valueBuffer.begin();  // points past the element that was last written */



void blinkLED(unsigned n=5, unsigned delay_=200) { 
    for (unsigned i = 0; i < n; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(delay_);
        digitalWrite(LED_BUILTIN, LOW);
        delay(delay_);
    }
}


void setup() {
    setStatus(DeviceStatus::BUSY);
    Serial.begin(9600);
    /* // wait until available */
    /* while (!Serial); */

    blinkLED(2);

    // TODO: test if works
    analogReadResolution(12);  // set analog input to 12 bit resolution (0-4095)

    if (!BLE.begin()) {
        Serial.println("starting Bluetooth® Low Energy module failed!");
        setStatus(DeviceStatus::ERROR);
        while(true);
    }
    /* BLE.setDeviceName("ArduinoTENG"); */
    BLE.setLocalName("ArduinoTENG");

    byte data[10] = "\xFF\xFFMQU@TUM";  // 0xFFFF manufacturer id for testing
    BLE.setManufacturerData(data, 10);

    initServices();
    BLE.addService(tengService);
    BLE.setAdvertisedService(tengService);

    // defaults
    tengCount.writeValue(100);
    tengInterval.writeValue(100);

    BLE.setConnectable(true);
    blinkLED(3);
    BLE.advertise();

    setStatus(DeviceStatus::WAIT_CONNECT);
}




/* constexpr int max_val = 1023 / 5 * 3.5;  // 1023 max val for 5V */
// it seems this function must return immediately, otherwise the connection will time out
void commandWrittenHandler(BLEDevice central, BLECharacteristic characteristic) {
    switch (tengCommand.value()) {
        case Command::STOP:
            measurementTask = STOP_MEASURE;
            break;
        case Command::MEASURE:
            if (measurementTask == STOP_MEASURE) {
                measurementTask = START_MEASURE;
            }
            else {
                Serial.println("ERROR: Command 'MEASURE' received while measurement is active");
            }
            break;
        case Command::MEASURE_COUNT:
            if (measurementTask == STOP_MEASURE) {
                measurementTask = START_MEASURE_COUNT;
            }
            else {
                Serial.println("ERROR: Command 'MEASURE_COUNT' received while measurement is active");
            }
            break;
        /* case Command::MEASURE_BASELINE: */
        /*     setStatus(DeviceStatus::MEASURING_BASELINE); */
        /*     measureBaseline(100); */
        /*     break; */
        default:
            setStatus(DeviceStatus::ERROR);
            Serial.print("ERROR: Unkown command: ");
            Serial.println(tengCommand.value(), HEX);
            delay(1000);
            break;
    }
}

// the loop function runs over and over again forever
void loop() {
    blinkLED(3, 300);
     // listen for Bluetooth® Low Energy peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        setStatus(DeviceStatus::CONNECTED);
        tengCommand.setEventHandler(BLEWritten, commandWrittenHandler);

        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            blinkLED(1, 200);
            if (measurementTask == START_MEASURE) {
                measure(central, false);
            }
            else if (measurementTask == START_MEASURE_COUNT) {
                measure(central, true);
            }

        }

        setStatus(DeviceStatus::WAIT_CONNECT);
        // when the central disconnects, notify the user
        Serial.print("Disconnected from central MAC: ");
        Serial.println(central.address());
    }
    else {
        BLE.advertise();
    }
}
