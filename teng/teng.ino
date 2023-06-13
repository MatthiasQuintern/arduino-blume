/*
 * EEPROM PROGRAMMER
 * 15-18 Address Pins
 * 8 Data Pins
 * 2 Control Pins (OEb, WEb)
 * 
 * CEb must tied to ground
 */

#include <Arduino.h>
#include <ArduinoBLE.h>
/* #include "eeprom.hpp" */
/* #include "address.hpp" */

/* #include "control_bytes.hpp" */

constexpr int LED_RED = 2;
constexpr int LED_YELLOW = 3;
constexpr int LED_GREEN = 4;
constexpr int PIN_TENG = 0;

/* void maskedWrite(uint32_t mask, uint32_t bits) { */
/*   for (uint32_t i = 0; i < 32; i++) { */
/*       if (mask & (1 << i)) { */
/*           digitalWrite(i, bits & (1 << i)); */
/*       } */
/*   } */
/* } */ 
void blinkLED(unsigned n=5, unsigned delay_=200) { 
    for (unsigned i = 0; i < n; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(delay_);
        digitalWrite(LED_BUILTIN, LOW);
        delay(delay_);
    }
}


unsigned MAX_DEVIATION = 0;
unsigned BASELINE = 0;
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
    /* pinMode(LED_BUILTIN, OUTPUT); */
    /* digitalWrite(LED_BUILTIN, HIGH); */
    /* delay(200); */

    Serial.begin(9600);
    /* // wait until available */
    while (!Serial);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200); // empty buffer
    while (Serial.read() != -1);

    /* digitalWrite(LED_BUILTIN, HIGH); */
    /* delay(5000); */

    /* // reset counter */
    /* blinkLED(); */

    /* uint8_t payload[] = { 1, 2, 3, 4, 5, 69 }; */
    /* uint8_t type = '#'; */
    /* static_assert(sizeof(payload) == 6); */
    /* uint16_t size = sizeof(payload); */
    /* Serial.write(type); */
    /* Serial.write(lowByte(size)); */
    /* Serial.write(highByte(size));u */
    /* Serial.write(payload, size); */

    /* delay(5000); */
    blinkLED(4);
    if (!BLE.begin()) {
        Serial.println("starting Bluetooth® Low Energy module failed!");
    }
    BLE.setDeviceName("ArduinoTeng");
    BLE.advertise();

    measureBaseline(100);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    delay(500);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
}



/* constexpr int max_val = 1023 / 5 * 3.5;  // 1023 max val for 5V */

// the loop function runs over and over again forever
void loop() {
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
    /* AddressState address(Counter(), eeprom.addressPinCount, eeprom.addressPins); */
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
