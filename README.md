# arduino-blume
**Me**asure analog signal using an Arduino and **Blu**etooth communication.
This program was written for (and tested with) the **Arduino RP2040 Connect**.

[m-teng](https://github.com/MatthiasQuintern/m-teng) has a backend that can connect to the Arduino.

## Configuration
See `settings.hpp` for which pin needs to be connected.
The program uses 3 leds for displaying device status, however these are optional.

## Installation
```
git clone https://github.com/MatthiasQuintern/arduino-blume
arduino-cli compile --profile nanorp
arduino-cli upload --profile nanorp
```

## Communication via Bluetooth
The Arduino will advertise a Bluetooth Low Energy service which can be used to interact with the device. 

Service UUID: `00010000-9a74-4b30-9361-4a16ec09930f`

0. status (`uint8_t`, `rn`): `00010001-9a74-4b30-9361-4a16ec09930f`
    - `0`: **ERROR**: error occured: invalid command received, can not advertise Bluetooth service, etc.
    - `1`: **BUSY**
    - `2`: **WAIT_CONNECT**: waiting for Bluetooth connection
    - `3`: **CONNECTED**: connected, waiting for command
    - `4`: **MEASURING**: connected, measurement running
1. command (`uint8_t`, `rw`): `00010002-9a74-4b30-9361-4a16ec09930f`
    send a command to the arduino:
    - `0` - **STOP**: stop active measurements
    - `1` - **MEASURE_COUNT**: measure *count* measurements with *interval* *see `count` and `interva` below*
    - `2` - **MEASURE**: measure with *interval* until interrupted *see `count` below*
2. reading (`uint16_t` `rn`): `00010003-9a74-4b30-9361-4a16ec09930f`
    the reading from the Arduinos analog pin
3. count: (`uint16_t`, `rw`): `00010004-9a74-4b30-9361-4a16ec09930f`
    set the number of measurements to take with `MEASURE_COUNT`
4. interval: (`uint16_t`, `rw`): `00010005-9a74-4b30-9361-4a16ec09930f`
    set the interval between measurements in ms
