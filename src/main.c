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

//------------------------------------------------------------------------------
// Functions

int main(void) {
    SYS_Initialize(NULL);

    // Main program loop
    while (true) {
        SYS_Tasks();
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
