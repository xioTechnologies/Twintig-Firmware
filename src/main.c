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
#include "NeoPixels/NeoPixels.h"
#include "ResetCause/ResetCause.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "Timer/Timer.h"
#include "Uart/Uart3.h"

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

    GPIO_PinSet(ENABLE_PIN);

    // Main program loop
    while (true) {
        SYS_Tasks();
        const NeoPixelsPixel colours[] = {
            {.rgb = 0xFF0000},
            {.rgb = 0x00FF00},
            {.rgb = 0x0000FF},
            {.rgb = 0xFFFFFF}
        };
        for (int index = 0; index < 4; index++) {
            NeoPixelsSet(&colours[index]);
            TimerDelayMilliseconds(500);
        }
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
