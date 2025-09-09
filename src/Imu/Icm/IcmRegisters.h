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
// Definitions

#define ICM_DEVICE_CONFIG_ADDRESS       (0x11)
#define ICM_INT_CONFIG_ADDRESS          (0x14)
#define ICM_TEMP_DATA1_ADDRESS          (0x1D)
#define ICM_INTF_CONFIG0_ADDRESS        (0x4C)
#define ICM_PWR_MGMT0_ADDRESS           (0x4E)
#define ICM_GYRO_CONFIG0_ADDRESS        (0x4F)
#define ICM_ACCEL_CONFIG0_ADDRESS       (0x50)
#define ICM_INT_CONFIG1_ADDRESS         (0x64)
#define ICM_INT_SOURCE0_ADDRESS         (0x65)
#define ICM_WHO_AM_I_ADDRESS            (0x75)

#define ICM_DEVICE_CONFIG_RESET_VALUE   (0x00)
#define ICM_INTF_CONFIG0_RESET_VALUE    (0x30)
#define ICM_INT_CONFIG_RESET_VALUE      (0x00)
#define ICM_INT_CONFIG1_RESET_VALUE     (0x10)
#define ICM_INT_SOURCE0_RESET_VALUE     (0x10)
#define ICM_GYRO_CONFIG0_RESET_VALUE    (0x06)
#define ICM_ACCEL_CONFIG0_RESET_VALUE   (0x06)
#define ICM_PWR_MGMT0_RESET_VALUE       (0x00)
#define ICM_WHO_AM_I_RESET_VALUE        (0x47)

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
        unsigned IntAysncReset : 1;
        unsigned IntTdeassertDisable : 1;
        unsigned IntTpulseDuration : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntConfig1Register;

typedef union {

    struct {
        unsigned UiAgcRdyInt1En : 1;
        unsigned FifoFullInt1En : 1;
        unsigned FifoThsInt1En : 1;
        unsigned UiDrdyInt1En : 1;
        unsigned ResetDoneInt1En : 1;
        unsigned PllRdyInt1En : 1;
        unsigned UiFsyncInt1En : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntSource0Register;

#endif

//------------------------------------------------------------------------------
// End of file
