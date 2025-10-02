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
#include "I2C/I2CBB2.h"
#include "I2C/I2CBB3.h"
#include "I2C/I2CBB4.h"
#include "I2C/I2CBB5.h"
#include "I2C/I2CBB6.h"
#include "I2C/I2CBB7.h"
#include "Imu/Icm/Icm1.h"
#include "Imu/Icm/Icm10.h"
#include "Imu/Icm/Icm11.h"
#include "Imu/Icm/Icm12.h"
#include "Imu/Icm/Icm13.h"
#include "Imu/Icm/Icm14.h"
#include "Imu/Icm/Icm15.h"
#include "Imu/Icm/Icm16.h"
#include "Imu/Icm/Icm17.h"
#include "Imu/Icm/Icm18.h"
#include "Imu/Icm/Icm19.h"
#include "Imu/Icm/Icm2.h"
#include "Imu/Icm/Icm20.h"
#include "Imu/Icm/Icm3.h"
#include "Imu/Icm/Icm4.h"
#include "Imu/Icm/Icm5.h"
#include "Imu/Icm/Icm6.h"
#include "Imu/Icm/Icm7.h"
#include "Imu/Icm/Icm8.h"
#include "Imu/Icm/Icm9.h"
#include "Imu/Imu.h"
#include "Led/Led.h"
#include "NeoPixels/NeoPixels.h"
#include "Notification/Notification.h"
#include "ResetCause/ResetCause.h"
#include "Spi/Spi1DmaTx.h"
#include "Spi/Spi2.h"
#include "Spi/Spi3Dma.h"
#include "Spi/Spi4Dma.h"
#include "Spi/Spi5.h"
#include "Spi/Spi6Dma.h"
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
    ResetCausePrint(ResetCauseGet());
    printf("Twintig " FIRMWARE_VERSION "\n");

    // Initialise modules
    TimerInitialise();
    I2CBB1BusClear();
    I2CBB2BusClear();
    I2CBB3BusClear();
    I2CBB4BusClear();
    I2CBB5BusClear();
    I2CBB6BusClear();
    I2CBB7BusClear();
    I2C1Initialise(I2CClockFrequency100kHz);
    I2C2Initialise(I2CClockFrequency100kHz);
    I2C3Initialise(I2CClockFrequency100kHz);
    I2C4Initialise(I2CClockFrequency100kHz);
    I2C5Initialise(I2CClockFrequency100kHz);
    Spi1DmaTxInitialise(&neoPixelsSpiSettings);
    Spi2Initialise(&icmSpiSettings);
    Spi3DmaInitialise(&icmSpiSettings);
    Spi4DmaInitialise(&icmSpiSettings);
    Spi5Initialise(&icmSpiSettings);
    Spi6DmaInitialise(&icmSpiSettings);
    LedInitialise();
    HapticInitialise();
    Ximu3DeviceInitialise();

    // Print self-test results
    printf("Haptic          %s\n", HapticTestResultToString(HapticTest()));
    printf("Carpus EEPROM   %s\n", EepromTestResultToString(EepromTest(&i2cBB1)));
    printf("CH1 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c3)));
    printf("CH2 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c2)));
    printf("CH3 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c5)));
    printf("CH4 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c1)));
    printf("CH5 EEPROM      %s\n", EepromTestResultToString(EepromTest(&i2c4)));
    printf("CH1 IMU1        %s\n", IcmTestResultToString(Icm1Test()));
    printf("CH1 IMU2        %s\n", IcmTestResultToString(Icm2Test()));
    printf("CH1 IMU3        %s\n", IcmTestResultToString(Icm3Test()));
    printf("CH1 IMU4        %s\n", IcmTestResultToString(Icm4Test()));
    printf("CH2 IMU1        %s\n", IcmTestResultToString(Icm5Test()));
    printf("CH2 IMU2        %s\n", IcmTestResultToString(Icm6Test()));
    printf("CH2 IMU3        %s\n", IcmTestResultToString(Icm7Test()));
    printf("CH2 IMU4        %s\n", IcmTestResultToString(Icm8Test()));
    printf("CH3 IMU1        %s\n", IcmTestResultToString(Icm9Test()));
    printf("CH3 IMU2        %s\n", IcmTestResultToString(Icm10Test()));
    printf("CH3 IMU3        %s\n", IcmTestResultToString(Icm11Test()));
    printf("CH3 IMU4        %s\n", IcmTestResultToString(Icm12Test()));
    printf("CH4 IMU1        %s\n", IcmTestResultToString(Icm13Test()));
    printf("CH4 IMU2        %s\n", IcmTestResultToString(Icm14Test()));
    printf("CH4 IMU3        %s\n", IcmTestResultToString(Icm15Test()));
    printf("CH4 IMU4        %s\n", IcmTestResultToString(Icm16Test()));
    printf("CH5 IMU1        %s\n", IcmTestResultToString(Icm17Test()));
    printf("CH5 IMU2        %s\n", IcmTestResultToString(Icm18Test()));
    printf("CH5 IMU3        %s\n", IcmTestResultToString(Icm19Test()));
    printf("CH5 IMU4        %s\n", IcmTestResultToString(Icm20Test()));

    // Main program loop
    while (true) {
        SYS_Tasks();

        // Application tasks
        ImuTasks(&imu1);
        ImuTasks(&imu2);
        ImuTasks(&imu3);
        ImuTasks(&imu4);
        ImuTasks(&imu5);
        ImuTasks(&imu6);
        ImuTasks(&imu7);
        ImuTasks(&imu8);
        ImuTasks(&imu9);
        ImuTasks(&imu10);
        ImuTasks(&imu11);
        ImuTasks(&imu12);
        ImuTasks(&imu13);
        ImuTasks(&imu14);
        ImuTasks(&imu15);
        ImuTasks(&imu16);
        ImuTasks(&imu17);
        ImuTasks(&imu18);
        ImuTasks(&imu19);
        ImuTasks(&imu20);
        NotificationTasks();
        UsbCdcTasks();
        Ximu3DeviceTasks();
    }
    return (EXIT_FAILURE);
}

//------------------------------------------------------------------------------
// End of file
