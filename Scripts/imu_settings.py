import json
from fnmatch import fnmatch

import cliny as cli
import ximu3_helpers

try:
    twintig_connection = ximu3_helpers.quick_connect("Twintig")

    imu_connections = ximu3_helpers.mux_connect(twintig_connection, 20, dictionary=True)

    with open("imu_settings.json") as file:
        for script in json.load(file):
            for connection in (c for n, c in imu_connections.items() if fnmatch(n, script["name"])):
                for command in script["commands"]:
                    ximu3_helpers.send_command(connection, json=command)

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
