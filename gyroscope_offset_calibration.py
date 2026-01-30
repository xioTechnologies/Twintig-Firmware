import time

import numpy as np
import ximu3

devices = ximu3.PortScanner.scan_filter(ximu3.PORT_TYPE_USB)

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

    def close(self) -> None:
        self.__connection.close()

    def send_command(self, command: str) -> None:
        response = self.__connection.send_command(command)

        if not response:
            raise Exception(f"No response. {command} for {self.__connection.get_config()}")

        if response.error:
            raise Exception(f"{response.error}. {command} for {self.__connection.get_config()}")

    def start_sampling(self):
        self.__gyroscope_offset = np.zeros(3)
        self.__number_of_samples = 0

        self.__callback_id = self.__connection.add_inertial_callback(self.__inertial_callback)

    def stop_sampling(self) -> np.ndarray:
        self.__connection.remove_callback(self.__callback_id)

        return self.__gyroscope_offset / self.__number_of_samples

    def __inertial_callback(self, message: ximu3.InertialMessage) -> None:
        self.__gyroscope_offset += np.array([message.gyroscope_x, message.gyroscope_y, message.gyroscope_z])
        self.__number_of_samples += 1


imu_connections = [ImuConnection(c) for c in [ximu3.MuxConnectionConfig(c, carpus_connection) for c in range(0x41, 0x55)]]

for imu_connection in imu_connections:
    imu_connection.send_command('{"factory":null}')
    imu_connection.send_command('{"gyroscope_offset":[0, 0, 0]}')
    imu_connection.send_command('{"gyroscope_offset_correction_enabled":false}')
    imu_connection.send_command('{"apply":null}')

    imu_connection.start_sampling()

time.sleep(10)

for imu_connection in imu_connections:
    offset = imu_connection.stop_sampling()

    imu_connection.send_command(f'{{"gyroscope_offset":{np.array2string(offset, separator=", ")}}}')
    imu_connection.send_command('{"save":null}')

    imu_connection.close()

carpus_connection.close()

print("Complete")
