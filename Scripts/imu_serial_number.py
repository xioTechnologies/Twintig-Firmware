import time

import cliny as cli
import ximu3_helpers

try:
    carpus_connection = ximu3_helpers.quick_connect("Twintig")

    imu_connections = ximu3_helpers.mux_connect(carpus_connection, 20)

    unix_timestamp = int(time.time())

    for ch_index in range(5):
        serial_number = ximu3_helpers.send_command(imu_connections[4 * ch_index], "serial_number")

        if serial_number != "Unknown":
            cli.print_warning(f"CH{ch_index + 1} already has serial number {serial_number}")
            continue

        for imu_index in range(4):
            connection = imu_connections[(4 * ch_index) + imu_index]

            ximu3_helpers.send_command(connection, "factory")
            ximu3_helpers.send_command(connection, "serial_number", f"{(unix_timestamp + ch_index):08X}-{imu_index + 1}")
            ximu3_helpers.send_command(connection, "hardware_version", "v1.0.4")  # edit this value as required
            ximu3_helpers.send_command(connection, "save")
            ximu3_helpers.send_command(connection, "blink")

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
