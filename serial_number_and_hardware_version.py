import time

import ximu3

devices = ximu3.PortScanner.scan()

time.sleep(1)  # wait for OS to release port

devices = [d for d in devices if "Carpus" in d.device_name]

if not devices:
    raise Exception("Carpus not found")

print(f"Found {devices[0]}")

connection = ximu3.Connection(devices[0].connection_config)

connection.open()

if connection.send_command('{"serial_number":null}').value != '"Unknown"':
    raise Exception("Device already has serial number")

commands = [
    '{"factory":null}',
    f'{{"serial_number":"{int(time.time()):08X}"}}',
    '{"hardware_version":"v1.0.1"}',  # modify this for hardware version
    '{"save":null}',
]

for response in connection.send_commands(commands):
    if not response:
        raise Exception("No response")

    if response.error:
        raise Exception(response.error)

connection.close()

print("Complete")
