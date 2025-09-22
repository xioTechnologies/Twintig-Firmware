/**
 * @file IcmRegisters.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM_REGISTERS_H
#define ICM_REGISTERS_H

//------------------------------------------------------------------------------
// Includes

#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions - Register bank 0

#define ICM_DEVICE_CONFIG_ADDRESS       (0x11)
#define ICM_INT_CONFIG_ADDRESS          (0x14)
#define ICM_TEMP_DATA1_ADDRESS          (0x1D)
#define ICM_INTF_CONFIG0_ADDRESS        (0x4C)
#define ICM_INTF_CONFIG1_ADDRESS        (0x4D)
#define ICM_PWR_MGMT0_ADDRESS           (0x4E)
#define ICM_GYRO_CONFIG0_ADDRESS        (0x4F)
#define ICM_ACCEL_CONFIG0_ADDRESS       (0x50)
#define ICM_INT_CONFIG1_ADDRESS         (0x64)
#define ICM_INT_SOURCE0_ADDRESS         (0x65)
#define ICM_WHO_AM_I_ADDRESS            (0x75)
#define ICM_REG_BANK_SEL_ADDRESS        (0x76)

#define ICM_DEVICE_CONFIG_RESET_VALUE   (0x00)
#define ICM_INTF_CONFIG0_RESET_VALUE    (0x30)
#define ICM_INT_CONFIG_RESET_VALUE      (0x00)
#define ICM_INT_CONFIG1_RESET_VALUE     (0x10)
#define ICM_INT_SOURCE0_RESET_VALUE     (0x10)
#define ICM_GYRO_CONFIG0_RESET_VALUE    (0x06)
#define ICM_ACCEL_CONFIG0_RESET_VALUE   (0x06)
#define ICM_PWR_MGMT0_RESET_VALUE       (0x00)
#define ICM_WHO_AM_I_RESET_VALUE        (0x47)
#define ICM_INTF_CONFIG1_RESET_VALUE    (0x91)

typedef union {

    struct {
        unsigned softResetConfig : 1;
        unsigned : 3;
        unsigned spiMode : 1;
        unsigned : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmDeviceConfigRegister;

typedef union {

    struct {
        unsigned int1Polarity : 1;
        unsigned int1DriveCircuit : 1;
        unsigned int1Mode : 1;
        unsigned int2Polarity : 1;
        unsigned int2DriveCircuit : 1;
        unsigned int2Mode : 1;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntConfigRegister;

typedef struct {
    int16_t tempData;
    int16_t accelDataX;
    int16_t accelDataY;
    int16_t accelDataZ;
    int16_t gyroDataX;
    int16_t gyroDataY;
    int16_t gyroDataZ;
} __attribute__((__packed__)) IcmSensorRegisters;

typedef union {

    struct {
        unsigned UiSifsCfg : 2;
        unsigned : 2;
        unsigned sensorDataEndian : 1;
        unsigned fifoCountEndian : 1;
        unsigned fifoCountRec : 1;
        unsigned fifoHoldLastDataEn : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntfConfig0Register;

typedef union {

    struct {
        unsigned clksel : 2;
        unsigned rtcMode : 1;
        unsigned accelLpClkSel : 1;
        unsigned : 4;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntfConfig1Register;

typedef union {

    struct {
        unsigned accelMode : 2;
        unsigned gyroMode : 2;
        unsigned idle : 1;
        unsigned tempDis : 1;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IcmPwrMgmt0Register;

typedef union {

    struct {
        unsigned gyroOdr : 4;
        unsigned : 1;
        unsigned gyroFsSel : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfig0Register;

typedef union {

    struct {
        unsigned accelOdr : 4;
        unsigned : 1;
        unsigned accelFsSel : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmAccelConfig0Register;

typedef union {

    struct {
        unsigned : 4;
        unsigned intAysncReset : 1;
        unsigned intTdeassertDisable : 1;
        unsigned intTpulseDuration : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntConfig1Register;

typedef union {

    struct {
        unsigned uiAgcRdyInt1En : 1;
        unsigned fifoFullInt1En : 1;
        unsigned fifoThsInt1En : 1;
        unsigned uiDrdyInt1En : 1;
        unsigned resetDoneInt1En : 1;
        unsigned pllRdyInt1En : 1;
        unsigned uiFsyncInt1En : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntSource0Register;

//------------------------------------------------------------------------------
// Definitions - Register bank 1

#define ICM_GYRO_CONFIG_STATIC2_ADDRESS     (0x0B)
#define ICM_GYRO_CONFIG_STATIC3_ADDRESS     (0x0C)
#define ICM_GYRO_CONFIG_STATIC4_ADDRESS     (0x0D)
#define ICM_GYRO_CONFIG_STATIC5_ADDRESS     (0x0E)
#define ICM_INTF_CONFIG5_ADDRESS            (0x7B)

#define ICM_GYRO_CONFIG_STATIC2_RESET_VALUE (0xA0)
#define ICM_GYRO_CONFIG_STATIC3_RESET_VALUE (0x0D)
#define ICM_GYRO_CONFIG_STATIC4_RESET_VALUE (0xAA)
#define ICM_GYRO_CONFIG_STATIC5_RESET_VALUE (0x80)
#define ICM_INTF_CONFIG5_RESET_VALUE        (0x00)

typedef union {

    struct {
        unsigned gyroNfDis : 1;
        unsigned gyroAafDis : 1;
        unsigned : 6;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfigStatic2Register;

typedef union {

    struct {
        unsigned gyroAafDelt : 6;
        unsigned : 2;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfigStatic3Register;

typedef union {

    struct {
        unsigned gyroAafDeltsqrLsb : 8;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfigStatic4Register;

typedef union {

    struct {
        unsigned gyroAafDeltsqrMsb : 4;
        unsigned gyroAafBitshift : 4;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfigStatic5Register;

typedef union {

    struct {
        unsigned : 1;
        unsigned pin9Function : 2;
        unsigned : 5;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntfConfig5Register;

//------------------------------------------------------------------------------
// Definitions - Register bank 2

#define ICM_ACCEL_CONFIG_STATIC2_ADDRESS        (0x03)
#define ICM_ACCEL_CONFIG_STATIC3_ADDRESS        (0x04)
#define ICM_ACCEL_CONFIG_STATIC4_ADDRESS        (0x05)

#define ICM_ACCEL_CONFIG_STATIC2_RESET_VALUE    (0x30)
#define ICM_ACCEL_CONFIG_STATIC3_RESET_VALUE    (0x40)
#define ICM_ACCEL_CONFIG_STATIC4_RESET_VALUE    (0x62)

typedef union {

    struct {
        unsigned accelAafDis : 1;
        unsigned accelAafDelt : 6;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmAccelConfigStatic2Register;

typedef union {

    struct {
        unsigned accelAafDeltsqrLsb : 8;
    } __attribute__((__packed__));
    uint8_t value;
} IcmAccelConfigStatic3Register;

typedef union {

    struct {
        unsigned accelAafDeltsqrMsb : 4;
        unsigned accelAafBitshift : 4;
    } __attribute__((__packed__));
    uint8_t value;
} IcmAccelConfigStatic4Register;

#endif

//------------------------------------------------------------------------------
// End of file
