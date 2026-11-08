import cliny as cli
import ximu3

try:
    if not cli.yes_or_no("Are you sure?"):
        raise RuntimeError("Aborted by user")

    twintig_connection = ximu3.helpers.connect("Twintig")

    imu_connections = ximu3.helpers.mux_connect_list(twintig_connection, 20)

    for connection in [twintig_connection] + imu_connections:
        ximu3.helpers.send_command(connection, "factory")
        ximu3.helpers.send_command(connection, "erase", timeout=5000)
        ximu3.helpers.send_command(connection, "blink")

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
