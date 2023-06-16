import bleak as b
import asyncio

TARGET_NAME = "ArduinoTENG"

TENG_SUUID          = "00010000-9a74-4b30-9361-4a16ec09930f"
TENG_STATUS_CUUID   = "00010001-9a74-4b30-9361-4a16ec09930f"
TENG_COMMAND_CUUID  = "00010002-9a74-4b30-9361-4a16ec09930f"
TENG_READING_CUUID  = "00010003-9a74-4b30-9361-4a16ec09930f"

TENG_COMMANDS = {
    "NOOP":             int(0).to_bytes(1),
    "MEASURE_BASELINE": int(1).to_bytes(1),
}
TENG_STATUS = ["ERROR", "BUSY", "WAIT_CONNECT", "MEASURING_BASELINE", "READING"]



def teng_status_callback(characteristic, data):
    value = int.from_bytes(data, byteorder="big", signed=False)
    if 0 <= value and value < len(TENG_STATUS):
        print(f"Status change: {TENG_STATUS[value]}")
    else:
        print(f"Status change (invalid): status={value}")

async def main():
    devices = await b.BleakScanner.discover(return_adv=True)
    # print(devices)
    target_device = None
    for adr, (device, adv_data) in devices.items():
        if device.name == TARGET_NAME:
            print(adv_data)
            target_device = device
            break
    if target_device is None:
        print("ERROR: Could not find target device")
        return
    print(f"Found target device: {target_device.name}: {target_device.metadata}, {target_device.details}")
    print(target_device.name, target_device.details)
    try:
        async with b.BleakClient(target_device) as client:
            for service in client.services:
                print(f"Service: {service.uuid}: {service.description}")
                for c in service.characteristics:
                    print(f"\t{c.uuid}: {c.properties}, {c.descriptors}")
            teng_status     = client.services.get_characteristic(TENG_STATUS_CUUID)
            teng_command    = client.services.get_characteristic(TENG_COMMAND_CUUID)
            teng_reading    = client.services.get_characteristic(TENG_READING_CUUID)
            client.start_notify(teng_status, teng_status_callback)

            await client.write_gatt_char(teng_command, TENG_COMMANDS["NOOP"])
            await asyncio.sleep(5)
            await client.write_gatt_char(teng_command, TENG_COMMANDS["MEASURE_BASELINE"])

            while client.is_connected:
                data = await client.read_gatt_char(teng_reading)


                value = int.from_bytes(data, byteorder="little", signed=False)
                print(f"Reading: {value}")
                await asyncio.sleep(0.5)
    except KeyboardInterrupt:
        pass
    except asyncio.exceptions.CancelledError:
        pass
    print("Disconnected")


if __name__ == "__main__":
    asyncio.run(main())
