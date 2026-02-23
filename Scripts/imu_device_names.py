import cliny as cli
import ximu3
import ximu3_helpers

try:
    carpus_connection = ximu3_helpers.quick_connect("Twintig")

    mux_channels = {
        0x41: "Carpus IMU",
        0x42: "I Metacarpal IMU",
        0x43: "I Proximal IMU",
        0x44: "I Distal IMU",
        0x45: "II Metacarpal IMU",
        0x46: "II Proximal IMU",
        0x47: "II Middle IMU",
        0x48: "II Distal IMU",
        0x49: "III Metacarpal IMU",
        0x4A: "III Proximal IMU",
        0x4B: "III Middle IMU",
        0x4C: "III Distal IMU",
        0x4D: "IV Metacarpal IMU",
        0x4E: "IV Proximal IMU",
        0x4F: "IV Middle IMU",
        0x50: "IV Distal IMU",
        0x51: "V Metacarpal IMU",
        0x52: "V Proximal IMU",
        0x53: "V Middle IMU",
        0x54: "V Distal IMU",
    }

    imu_connections = {n: ximu3.Connection(ximu3.MuxConnectionConfig(c, carpus_connection)).open() for c, n in mux_channels.items()}

    for name, connection in imu_connections.items():
        ximu3_helpers.send_command(connection, "device_name", name)
        ximu3_helpers.send_command(connection, "blink")

    cli.print_success("Complete")

except Exception as ex:
    cli.print_error(ex)
