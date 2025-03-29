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
#include "NeoPixels/NeoPixels.h"
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
    NeoPixelsInitialise();
    HapticInitialise();
    Uart1Initialise(&uartSettingsDefault);

    GPIO_PinSet(ENABLE_PIN);
    GPIO_PinSet(ENABLE_CH1_PIN);
    GPIO_PinSet(ENABLE_CH2_PIN);
    GPIO_PinSet(ENABLE_CH3_PIN);
    GPIO_PinSet(ENABLE_CH4_PIN);
    GPIO_PinSet(ENABLE_CH5_PIN);

    // Main program loop
    while (true) {
        SYS_Tasks();
        UsbCdcTasks();
        Ximu3DeviceTasks();

        if (TIMER_SCHEDULER_POLL(0.5f)) {
            static const NeoPixelsPixel left[] = {
                {.rgb = 0x110011},
                {.rgb = 0x001111},
            };
            static const NeoPixelsPixel right[] = {
                {.rgb = 0x001111},
                {.rgb = 0x110011},
            };
            static int counter;
            NeoPixelsSet((counter++ & 0x1) ? left : right);
        }
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
