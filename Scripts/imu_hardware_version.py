import time

import ximu3

devices = ximu3.PortScanner.scan()

time.sleep(1)  # wait for OS to release port

devices = [d for d in devices if "Carpus" in d.device_name]

if not devices:
    raise Exception("Carpus not found")

print(f"Found {devices[0]}")

carpus_connection = ximu3.Connection(devices[0].connection_config)

carpus_connection.open()

imu_connections = [ximu3.Connection(c) for c in [ximu3.MuxConnectionConfig(c, carpus_connection) for c in range(0x41, 0x55)]]


def send_command(connection: ximu3.Connection, command: str) -> None:
    response = connection.send_command(command)

    if not response:
        raise Exception(f"No response. {command} for {connection.get_config()}")

    if response.error:
        raise Exception(f"{response.error}. {command} for {connection.get_config()}")


for imu_connection in imu_connections:
    imu_connection.open()

    send_command(imu_connection, '{"factory":null}')
    send_command(imu_connection, '{"hardware_version":"v1.0.4"}')  # edit this value as required
    send_command(imu_connection, '{"save":null}')
    send_command(imu_connection, '{"blink":null}')

    imu_connection.close()

carpus_connection.close()

print("Complete")
