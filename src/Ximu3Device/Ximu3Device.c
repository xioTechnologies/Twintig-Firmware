/**
 * @file Ximu3Device.c
 * @author Seb Madgwick
 * @brief x-IMU3 device.
 */

//------------------------------------------------------------------------------
// Includes

#include "Commands.h"
#include "Interfaces.h"
#include "Timer/Timer.h"
#include "x-IMU3-Device/Ximu3Command.h"
#include "x-IMU3-Device/Ximu3Data.h"

//------------------------------------------------------------------------------
// Function declarations

static void Error(const char* const error, void* const context);

//------------------------------------------------------------------------------
// Variables

static Ximu3CommandInterface interfaces[] = {
    {
        .name = "USB",
        .read = InterfacesUsbRead,
        .write = InterfacesUsbWrite,
    },
    {
        .name = "Serial",
        .read = InterfacesSerialRead,
        .write = InterfacesSerialWrite,
    },
};
static const Ximu3CommandMap commands[] = {
    {"ping", CommandsPing},
};
static Ximu3CommandBridge bridge = {
    .interfaces = interfaces,
    .numberOfInterfaces = sizeof (interfaces) / sizeof (Ximu3CommandInterface),
    .commands = commands,
    .numberOfCommands = sizeof (commands) / sizeof (Ximu3CommandMap),
    .error = Error,
    .context = NULL, // TODO: use context for MUX
};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 */
void Ximu3DeviceTasks(void) {
    Ximu3CommandTasks(&bridge);
}

/**
 * @brief Error handler.
 * @param error error.
 * @param context Context.
 */
static void Error(const char* const error, void* const context) {
    const Ximu3DataError data = {
        .timestamp = TimerGetTicks64() / TIMER_TICKS_PER_MICROSECOND,
        .string = error,
    };
    char message[256];
    const int numberOfBytes = Ximu3DataErrorAscii(message, sizeof (message), &data);
    InterfacesUsbWrite(message, numberOfBytes, context);
    InterfacesSerialWrite(message, numberOfBytes, context);
}

//------------------------------------------------------------------------------
// End of file
