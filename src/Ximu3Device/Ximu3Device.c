/**
 * @file Ximu3Device.c
 * @author Seb Madgwick
 * @brief x-IMU3 device.
 */

//------------------------------------------------------------------------------
// Includes

#include <stdio.h>
#include "Timer/Timer.h"
#include "Usb/UsbCdc.h"
#include "x-IMU3-Device/Ximu3Command.h"
#include "x-IMU3-Device/Ximu3Data.h"
#include "Uart/Uart1.h"

//------------------------------------------------------------------------------
// Function declarations

static void Ping(const char* * const value, Ximu3CommandResponse * const response);
static void Error(const char* const error);

//------------------------------------------------------------------------------
// Variables

static Ximu3CommandInterface interfaces[] = {
    {
        .name = "USB",
        .read = UsbCdcRead,
        .write = UsbCdcWrite,
    },
    {
        .name = "Serial",
        .read = Uart1Read,
        .write = Uart1Write,
    },
};
static const Ximu3CommandMap commands[] = {
    {"ping", Ping},
};
static Ximu3CommandBridge bridge = {
    .interfaces = interfaces,
    .numberOfInterfaces = sizeof (interfaces) / sizeof (Ximu3CommandInterface),
    .commands = commands,
    .numberOfCommands = sizeof (commands) / sizeof (Ximu3CommandMap),
    .error = Error,
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
 * @brief Ping command.
 * @param value Value.
 * @param response Response.
 */
static void Ping(const char* * const value, Ximu3CommandResponse * const response) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    Ximu3CommandRespondPing(response, "Twintig", "01234567");
}

/**
 * @brief Error handler.
 * @param error error.
 */
static void Error(const char* const error) {
    const Ximu3DataError data = {
        .timestamp = TimerGetTicks64() / TIMER_TICKS_PER_MICROSECOND,
        .string = error,
    };
    char message[256];
    const int numberOfBytes = Ximu3DataErrorAscii(message, sizeof (message), &data);
    UsbCdcWrite(message, numberOfBytes);
}

//------------------------------------------------------------------------------
// End of file
