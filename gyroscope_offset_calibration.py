import time

import numpy as np
import ximu3

devices = ximu3.PortScanner.scan()

time.sleep(0.1)  # wait for ports to close

devices = [d for d in devices if "Carpus" in d.device_name]

if not devices:
    raise Exception("Unable to find Carpus")

carpus_connection = ximu3.Connection(devices[0].connection_info)

result = carpus_connection.open()

if result != ximu3.RESULT_OK:
    raise Exception(f"Unable to open connection {carpus_connection.get_info().to_string()}. {ximu3.result_to_string(result)}")


class Connection:
    def __init__(self, connection_info: ximu3.MuxConnectionInfo) -> None:
        self.__connection = ximu3.Connection(connection_info)

        result = self.__connection.open()

        if result != ximu3.RESULT_OK:
            raise Exception(f"Unable to open {connection_info.to_string()}. {ximu3.result_to_string(result)}.")

    def send_command(self, command: str) -> None:
        responses = self.__connection.send_commands([command], 2, 500)

        if not responses:
            raise Exception(f"No response. {command} sent to {self.__connection.get_info().to_string()}")

        response = ximu3.CommandMessage.parse(responses[0])

        if response.error:
            raise Exception(f"{response.error}. {command} sent to {self.__connection.get_info().to_string()}")

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


imu_connections = [Connection(c) for c in [ximu3.MuxConnectionInfo(c, carpus_connection) for c in range(0x41, 0x55)]]

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
