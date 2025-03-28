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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "Timer/Timer.h"
#include "ResetCause/ResetCause.h"
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

    // Main program loop
    while (true) {
        SYS_Tasks();
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
