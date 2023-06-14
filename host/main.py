import bleak as b
import asyncio

TARGET_NAME = "ArduinoTENG"
TARGET_ADRESS = "C8:C9:A3:E6:08:3A"

def disconnect_callback(client: b.BleakClient):
    print(f"Disconnected {client}")

async def main():
    # devices = await b.BleakScanner.discover(return_adv=False)
    # # print(devices)
    # target_device = None
    # for adr, (device, adv_data) in devices.items():
    #     if device.name == TARGET_NAME:
    #         print(adv_data)
    #         target_device = device
    #         break
    # if target_device is None:
    #     print("ERROR: Could not find target device")
    #     return
    # print(f"Found target device: {target_device.name}: {target_device.metadata}, {target_device.details}")
    # print(target_device.name, target_device.details)
    # async with b.BleakClient(target_device) as client:
    #     # print(f"Connected to {client}")
    #     print(f"Services: {await client.services}")
    scanner = b.BleakScanner()
    scanner.start()
    target_device = TARGET_ADRESS
    client = b.BleakClient(target_device)
    try:
        await client.connect()
        # print(client.services)
    except b.BleakError as e:
        print(e)
    finally:
        await client.disconnect()
    scanner.stop()


if __name__ == "__main__":
    asyncio.run(main())
