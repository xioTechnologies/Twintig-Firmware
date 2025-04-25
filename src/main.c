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
#include "Imu/Icm/Icm1.h"
#include "Imu/Imu.h"
#include "Leds/Leds.h"
#include "Notification/Notification.h"
#include "ResetCause/ResetCause.h"
#include "Spi/Spi4Dma.h"
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
    Spi4DmaInitialise(&icmSpiSettings);
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
    printf("CH1 IMU4        %s\n", IcmTestResultToString(Icm1Test()));

    // Main program loop
    while (true) {
        SYS_Tasks();

        // Application tasks
        ImuTasks(&imu1);
        // ImuTasks(&imu2);
        // ImuTasks(&imu3);
        // ImuTasks(&imu4);
        // ImuTasks(&imu5);
        // ImuTasks(&imu6);
        // ImuTasks(&imu7);
        // ImuTasks(&imu8);
        // ImuTasks(&imu9);
        // ImuTasks(&imu10);
        // ImuTasks(&imu11);
        // ImuTasks(&imu12);
        // ImuTasks(&imu13);
        // ImuTasks(&imu14);
        // ImuTasks(&imu15);
        // ImuTasks(&imu16);
        // ImuTasks(&imu17);
        // ImuTasks(&imu18);
        // ImuTasks(&imu19);
        // ImuTasks(&imu20);
        NotificationTasks();
        UsbCdcTasks();
        Ximu3DeviceTasks();
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
