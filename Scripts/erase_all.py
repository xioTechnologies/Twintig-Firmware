import cliny as cli
import ximu3_helpers


try:
    if not cli.yes_or_no("Are you sure?"):
        raise RuntimeError("Aborted by user")

    carpus_connection = ximu3_helpers.quick_connect("Twintig")

    imu_connections = ximu3_helpers.mux_connect(carpus_connection, 20)

    for connection in [carpus_connection] + imu_connections:
        ximu3_helpers.send_command(connection, "factory")
        ximu3_helpers.send_command(connection, "erase", timeout=5000)
        ximu3_helpers.send_command(connection, "blink")

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
