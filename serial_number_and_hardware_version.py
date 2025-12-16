import time

import ximu3

devices = ximu3.PortScanner.scan()

time.sleep(0.1)  # wait for ports to close

devices = [d for d in devices if "Carpus" in d.device_name]

if not devices:
    raise Exception("Unable to find Carpus")

connection = ximu3.Connection(devices[0].connection_info)

result = connection.open()

if result != ximu3.RESULT_OK:
    raise Exception(f"Unable to open connection {connection.get_info().to_string()}. {ximu3.result_to_string(result)}")

commands = [
    '{"factory":null}',
    f'{{"serial_number":"{int(time.time()):08X}"}}',
    '{"hardware_version":"v1.0.1"}',  # modify this for hardware version
    '{"save":null}',
]

for response in connection.send_commands(commands, 2, 500):
    response = ximu3.CommandMessage.parse(response)

    if response.error:
        print(response.error)
        continue

    print(f"{response.key} : {response.value}")

connection.close()
