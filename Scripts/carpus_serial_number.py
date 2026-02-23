import time

import cliny as cli
import ximu3_helpers

try:
    connection = ximu3_helpers.quick_connect("Twintig")

    serial_number = ximu3_helpers.send_command(connection, "serial_number")

    if serial_number != "Unknown":
        raise RuntimeError(f"Device already has serial number {serial_number}")

    ximu3_helpers.send_command(connection, "factory")
    ximu3_helpers.send_command(connection, "serial_number", f"{int(time.time()):08X}")
    ximu3_helpers.send_command(connection, "hardware_version", "v1.0.1")  # edit this value as required
    ximu3_helpers.send_command(connection, "save")
    ximu3_helpers.send_command(connection, "blink")

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
