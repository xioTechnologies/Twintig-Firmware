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
#include "Eeprom/Eeprom.h"
#include "FirmwareVersion.h"
#include "Haptic/Haptic.h"
#include "I2C/I2C1.h"
#include "I2C/I2C2.h"
#include "I2C/I2C3.h"
#include "I2C/I2C4.h"
#include "I2C/I2C5.h"
#include "I2C/I2CBB1.h"
#include "Imu/Imu.h"
#include "Leds/Leds.h"
#include "Notification/Notification.h"
#include "ResetCause/ResetCause.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "Timer/Timer.h"
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
    printf("Twintig " FIRMWARE_VERSION "\n");

    // Initialise modules
    TimerInitialise();
    I2C1Initialise(I2CClockFrequency100kHz);
    I2C2Initialise(I2CClockFrequency100kHz);
    I2C3Initialise(I2CClockFrequency100kHz);
    I2C4Initialise(I2CClockFrequency100kHz);
    I2C5Initialise(I2CClockFrequency100kHz);
    LedsInitialise();
    HapticInitialise();
    Ximu3DeviceInitialise();

    printf("Haptic          %s\n", HapticTestResultToString(HapticTest()));
    printf("Carpus EEPROM   %s\n", EepromTestResultToString(EepromTest(&i2cBB1)));
    printf("CH1 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c3)));
    printf("CH2 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c2)));
    printf("CH3 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c5)));
    printf("CH4 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c1)));
    printf("CH5 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c4)));

    // Main program loop
    while (true) {
        SYS_Tasks();

        // Application tasks
        ImuTasks(&imu1);
        NotificationTasks();
        UsbCdcTasks();
        Ximu3DeviceTasks();
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
