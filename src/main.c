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
#include "Haptic/Haptic.h"
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
    printf("Twintig v0.0.0\n");

    // Initialise modules
    TimerInitialise();
    LedsInitialise();
    HapticInitialise();
    Uart1Initialise(&uartSettingsDefault);
    Ximu3DeviceInitialise();

    // Main program loop
    while (true) {
        SYS_Tasks();
        UsbCdcTasks();
        Ximu3DeviceTasks();
        if (TIMER_SCHEDULER_POLL(0.5f)) {
            printf(".");
        }
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
