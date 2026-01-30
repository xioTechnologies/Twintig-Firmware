import json
import time
from fnmatch import fnmatch

import ximu3

devices = ximu3.PortScanner.scan()

time.sleep(1)  # wait for OS to release port

devices = [d for d in devices if "Carpus" in d.device_name]

if not devices:
    raise Exception("Carpus not found")

print(f"Found {devices[0]}")

carpus_connection = ximu3.Connection(devices[0].connection_config)

carpus_connection.open()


class ImuConnection:
    def __init__(self, config: ximu3.MuxConnectionConfig) -> None:
        self.__connection = ximu3.Connection(config)

        self.__connection.open()

        response = self.__connection.ping()

        if not response:
            raise Exception(f"Ping failed for {self.__connection.get_config()}")

        self.__name = response.device_name

    def close(self) -> None:
        self.__connection.close()

    def send_command(self, command: str) -> None:
        response = self.__connection.send_command(command)

        if not response:
            raise Exception(f"No response. {command} for {self.__connection.get_config()}")

        if response.error:
            raise Exception(f"{response.error}. {command} for {self.__connection.get_config()}")

    @property
    def name(self) -> str:
        return self.__name


imu_connections = [ImuConnection(c) for c in [ximu3.MuxConnectionConfig(c, carpus_connection) for c in range(0x41, 0x55)]]

with open("imu_settings.json") as file:
    scripts = json.load(file)

for script in scripts:
    for imu_connection in [i for i in imu_connections if fnmatch(i.name, script["name"])]:
        for command in script["commands"]:
            imu_connection.send_command(command)

for imu_connection in imu_connections:
    imu_connection.close()

carpus_connection.close()

print("Complete")
