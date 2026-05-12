from datetime import datetime

import cliny as cli
import numpy as np
import ximu3
import ximu3csv

try:
    twintig_connection = ximu3.helpers.quick_connect("Twintig")

    imu_connections = ximu3.helpers.mux_connect(twintig_connection, 20, dictionary=True)

    for connection in imu_connections.values():
        ximu3.helpers.send_command(connection, "factory")
        ximu3.helpers.send_command(connection, "gyroscope_offset", [0, 0, 0])
        ximu3.helpers.send_command(connection, "gyroscope_bias_correction_enabled", False)
        ximu3.helpers.send_command(connection, "apply")

    data_logger = ximu3.helpers.DataLogger(list(imu_connections.values()), seconds=5, overwrite=True)

    devices = ximu3csv.read(data_logger.path)

    for device in devices:
        connection = imu_connections[device.device_name]

        offset: np.ndarray = device.inertial.gyroscope.xyz.mean(axis=0)

        ximu3.helpers.send_command(connection, "calibration_date", datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        ximu3.helpers.send_command(connection, "gyroscope_offset", offset.tolist())
        ximu3.helpers.send_command(connection, "save")
        ximu3.helpers.send_command(connection, "blink")

    data_logger.delete()

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
