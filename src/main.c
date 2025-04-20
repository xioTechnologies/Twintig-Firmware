/**
 * @file main.c
 * @author Seb Madgwick
 * @brief Main file.
 *
 * Device:
 * PIC32MZ2048EFH144
 *
 * Compiler:
 * XC32 v4.60, MPLAB Harmony 3
 */

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "FirmwareVersion.h"
#include "Haptic/Haptic.h"
#include "Imu/Imu.h"
#include "Leds/Leds.h"
#include "ResetCause/ResetCause.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "Timer/Timer.h"
#include "Timer/TimerScheduler.h"
#include "Uart/Uart1.h"
#include "Uart/Uart3.h"
#include "Usb/UsbCdc.h"
#include "Ximu3Device/x-IMU3-Device/Ximu3.h"
#include "Ximu3Device/Ximu3Device.h"

//------------------------------------------------------------------------------
// Functions

int main(void) {
    SYS_Initialize(NULL);

    // Initialise debug UART
    Uart3Initialise(&uartSettingsDefault);

    // Print start up message
    const RCON_RESET_CAUSE resetCause = ResetCauseGet();
    ResetCausePrint(resetCause);
    printf("Twintig " FIRMWARE_VERSION "\n");

    // Initialise modules
    TimerInitialise();
    LedsInitialise();
    HapticInitialise();
    Ximu3DeviceInitialise();

    ImuInitialise(ImuOdr32kHz);

    // Main program loop
    while (true) {
        SYS_Tasks();
        UsbCdcTasks();
        Ximu3DeviceTasks();

        ImuData imuData;
        while (ImuGetData(&imuData)) {
            const Ximu3DataInertial inertialData = {
                .timestamp = imuData.timestamp / TIMER_TICKS_PER_MICROSECOND,
                .gyroscopeX = imuData.gyroscopeX,
                .gyroscopeY = imuData.gyroscopeY,
                .gyroscopeZ = imuData.gyroscopeZ,
                .accelerometerX = imuData.accelerometerX,
                .accelerometerY = imuData.accelerometerY,
                .accelerometerZ = imuData.accelerometerZ,
            };
            uint8_t message[64];
            const size_t numberOfBytes = Ximu3DataInertialBinary(message, sizeof (message), &inertialData);
            UsbCdcWrite(message, numberOfBytes);
            Uart1Write(message, numberOfBytes);
            if (TIMER_SCHEDULER_POLL(0.2f)) {
                const Ximu3DataTemperature temperatureData = {
                    .timestamp = imuData.timestamp / TIMER_TICKS_PER_MICROSECOND,
                    .temperature = imuData.temperature,
                };
                uint8_t message[64];
                const size_t numberOfBytes = Ximu3DataTemperatureBinary(message, sizeof (message), &temperatureData);
                UsbCdcWrite(message, numberOfBytes);
                Uart1Write(message, numberOfBytes);
            }
        }
        if (TIMER_SCHEDULER_POLL(1.0f)) {
            const Ximu3DataNotification notificationData = {
                .timestamp = imuData.timestamp / TIMER_TICKS_PER_MICROSECOND,
                .string = "Tick",
            };
            uint8_t message[64];
            const size_t numberOfBytes = Ximu3DataNotificationBinary(message, sizeof (message), &notificationData);
            UsbCdcWrite(message, numberOfBytes);
            Uart1Write(message, numberOfBytes);
        }
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
