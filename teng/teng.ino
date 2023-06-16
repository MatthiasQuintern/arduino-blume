/*
 * 
 */

#include <Arduino.h>
#include <ArduinoBLE.h>

#include "settings.hpp"
#include "status.hpp"
#include "services.hpp"

void blinkLED(unsigned n=5, unsigned delay_=200) { 
    for (unsigned i = 0; i < n; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(delay_);
        digitalWrite(LED_BUILTIN, LOW);
        delay(delay_);
    }
}



uint16_t MAX_DEVIATION = 0;
uint16_t BASELINE = 0;
void measureBaseline(unsigned nMeas, unsigned interval=50) {
    uint64_t value = 0;
    unsigned minVal = 1023;
    unsigned maxVal = 0;
    for (unsigned i = 0; i < nMeas; i++) {
        unsigned reading = analogRead(PIN_TENG);
        value += reading;
        delay(interval);
        if (reading > maxVal) { maxVal = reading; }
        if (reading < minVal) { minVal = reading; }
    }
    BASELINE = value / nMeas;
    if (BASELINE - minVal > maxVal - BASELINE) {
        MAX_DEVIATION = BASELINE - minVal;
    }
    else {
        MAX_DEVIATION = maxVal - BASELINE;
    }
}


void setup() {
    setStatus(DeviceStatus::BUSY);
    Serial.begin(9600);
    /* // wait until available */
    /* while (!Serial); */

    blinkLED(2);

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

    BLE.setConnectable(true);
    blinkLED(3);
    BLE.advertise();

    setStatus(DeviceStatus::WAIT_CONNECT);
}



/* constexpr int max_val = 1023 / 5 * 3.5;  // 1023 max val for 5V */

// the loop function runs over and over again forever
void loop() {
    blinkLED(3, 100);
     // listen for Bluetooth® Low Energy peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        setStatus(DeviceStatus::READING);
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            if (tengCommand.written()) {
                switch (tengCommand.value()) {
                    case Command::NOOP:
                        setStatus(DeviceStatus::BUSY);
                        delay(1000);
                        break;
                    case Command::MEASURE_BASELINE:
                        setStatus(DeviceStatus::MEASURING_BASELINE);
                        measureBaseline(100);
                        break;
                    default:
                        setStatus(DeviceStatus::ERROR);
                        delay(1000);
                        break;
                }
                setStatus(DeviceStatus::READING);
            }
            int val = analogRead(PIN_TENG);
            tengReading.writeValue(static_cast<uint16_t>(val));
            Serial.print(val, DEC);
            val -= BASELINE;
            if (val < 0) { val = -val; }
            Serial.print("/");
            Serial.println(val, DEC);
            delay(300);
        }
        setStatus(DeviceStatus::WAIT_CONNECT);
        // when the central disconnects, notify the user
        Serial.print("Disconnected from central MAC: ");
        Serial.println(central.address());
    }
    else {
        BLE.advertise();
    }
    return;

    int led_red = LOW;
    int led_yellow = LOW;
    int led_green = LOW;

    int val = analogRead(PIN_TENG);
    Serial.print(val, DEC);
    val -= BASELINE;
    if (val < 0) { val = -val; }
    Serial.print("/");
    Serial.println(val, DEC);

    if (val >= MAX_DEVIATION * 1) {
        led_green = HIGH;
    }
    if (val >= MAX_DEVIATION * 2) {
        led_yellow = HIGH;
    }
    if (val >= MAX_DEVIATION * 3) {
        led_red = HIGH;
    }
    digitalWrite(LED_RED, led_red);
    digitalWrite(LED_YELLOW, led_yellow);
    digitalWrite(LED_GREEN, led_green);

    delay(100);


    
    /* // init address */
    /* const int* addressPins = nullptr; */
    /* unsigned addressPinCount = 0; */
    /* // TODO pass eeprom via serial */
    /* EEPROM eeprom = AT28C256; */
    /* if (mem_size = CTRL_2M) { */
    /*     eeprom = SST39SF02A; */
    /* } */
    /* AddressStatus address(Counter(), eeprom.addressPinCount, eeprom.addressPins); */
    /* const int dataSize = 0x8000; */
    /* uint8_t data[dataSize]; */

    /* while (true) { */

    /* } */
    /* if (!Serial.available()) { continue; } */

    /* // read packet */
    /* uint8_t type = Serial.read(); */
    /* uint16_t size = Serial.read();  // low byte */
    /* size |= (Serial.read() << 8);   // high byte */
    /* Packet packet(type, size); */
    /* Serial.readBytes(packet.payload, size); */

    /* switch(type) { */
    /*     case PacketType::WRITE: { */
    /*         break; */
    /*     } */
    /*     case PacketType::READ: { */
    /*         break; */
    /*     } */
    /*     case PacketType::SET_ADDRESS { */

    /*     } */
    /*     default: { */
    /*         break; */
    /*     } */
    /* } */



    
    /* byte mem_size = 0; */
    /* byte action = 0; */
    /* byte x = 0; */
    /* while (action == 0 || mem_size == 0) { */
    /*     digitalWrite(LED_BUILTIN, HIGH); */
    /*     if (Serial.available() <= 0) { */
    /*         blinkLED(3); */
    /*     } */
    /*     else { */
    /*         /1* while (Serial.available() > 0) { *1/ */
    /*         /1*     Serial.write(Serial.read()); *1/ */
    /*         /1* } *1/ */
    /*         /1* Serial.write('\n'); *1/ */

    /*         Serial.write(static_cast<uint8_t>(Serial.available())); */
    /*         int cmd = Serial.read(); */
    /*         switch (cmd) { */
    /*             case CTRL_WRITE: */
    /*             case CTRL_READ: */   
    /*                 action = cmd; */
    /*                 Serial.write('\0'); */
    /*                 Serial.write('\0'); */
    /*                 break; */
    /*             default: */
    /*                 mem_size = cmd; */
    /*                 Serial.write('\1'); */
    /*                 Serial.write('\1'); */
    /*         } */
    /*         Serial.write(static_cast<uint8_t>(cmd)); */
    /*         Serial.write('\2'); */
    /*         Serial.write('\2'); */
    /*     } */
    /*     Serial.write(x); */
    /*     Serial.print('\n'); */
    /*     x++; */
    /*     delay(1000); */
    /*     digitalWrite(LED_BUILTIN, LOW); */
    /*     delay(1000); */
    /* } */

    /* if (action == CTRL_WRITE) { */
    /*     Serial.println("Write"); */
    /* } */
    /* else if (action == CTRL_READ) { */
    /*     Serial.println("Read"); */
    /*     eeprom.read(data, dataSize, address); */
    /*     Serial.write(data, dataSize); */
    /*     blinkLED(10); */
    /* } */
}
