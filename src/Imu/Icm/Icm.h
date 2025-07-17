/**
 * @file Icm.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM_H
#define ICM_H

//------------------------------------------------------------------------------
// Includes

#include "Spi/Spi.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief DEVICE_CONFIG register address.
 */
#define ICM_DEVICE_CONFIG_ADDRESS (0x11)

/**
 * @brief INT_CONFIG register address.
 */
#define ICM_INT_CONFIG_ADDRESS (0x14)

/**
 * @brief TEMP_DATA1 register address.
 */
#define ICM_TEMP_DATA1_ADDRESS (0x1D)

/**
 * @brief INTF_CONFIG0 register address.
 */
#define ICM_INTF_CONFIG0_ADDRESS (0x4C)

/**
 * @brief PWR_MGMT0 register address.
 */
#define ICM_PWR_MGMT0_ADDRESS (0x4E)

/**
 * @brief GYRO_CONFIG0 register address.
 */
#define ICM_GYRO_CONFIG0_ADDRESS (0x4F)

/**
 * @brief ACCEL_CONFIG0 register address.
 */
#define ICM_ACCEL_CONFIG0_ADDRESS (0x50)

/**
 * @brief INT_CONFIG1 register address.
 */
#define ICM_INT_CONFIG1_ADDRESS (0x64)

/**
 * @brief INT_SOURCE0 register address.
 */
#define ICM_INT_SOURCE0_ADDRESS (0x65)

/**
 * @brief WHO_AM_I register address.
 */
#define ICM_WHO_AM_I_ADDRESS (0x75)

/**
 * @brief DEVICE_CONFIG reset value.
 */
#define  ICM_DEVICE_CONFIG_RESET_VALUE (0x00)

/**
 * @brief INTF_CONFIG0 reset value.
 */
#define ICM_INTF_CONFIG0_RESET_VALUE (0x30)

/**
 * @brief INT_CONFIG reset value.
 */
#define ICM_INT_CONFIG_RESET_VALUE (0x00)

/**
 * @brief INT_CONFIG1 reset value.
 */
#define  ICM_INT_CONFIG1_RESET_VALUE (0x10)

/**
 * @brief INT_SOURCE0 reset value.
 */
#define  ICM_INT_SOURCE0_RESET_VALUE (0x10)

/**
 * @brief GYRO_CONFIG0 reset value.
 */
#define ICM_GYRO_CONFIG0_RESET_VALUE (0x06)

/**
 * @brief ACCEL_CONFIG0 reset value.
 */
#define  ICM_ACCEL_CONFIG0_RESET_VALUE (0x06)

/**
 * @brief PWR_MGMT0 reset value.
 */
#define ICM_PWR_MGMT0_RESET_VALUE (0x00)

/**
 * @brief DEVICE_CONFIG register.
 */
typedef union {

    struct {
        unsigned softResetConfig : 1;
        unsigned : 3;
        unsigned spiMode : 1;
        unsigned : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmDeviceConfigRegister;

/**
 * @brief INT_CONFIG register.
 */
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

/**
 * @brief Sensor registers.
 */
typedef struct {
    int16_t tempData;
    int16_t accelDataX;
    int16_t accelDataY;
    int16_t accelDataZ;
    int16_t gyroDataX;
    int16_t gyroDataY;
    int16_t gyroDataZ;
} __attribute__((__packed__)) IcmSensorRegisters;

/**
 * @brief INTF_CONFIG0 register.
 */
typedef union {

    struct {
        unsigned UISifsCfg : 2;
        unsigned : 2;
        unsigned sensorDataEndian : 1;
        unsigned fifoCountEndian : 1;
        unsigned fifoCountRec : 1;
        unsigned fifoHoldLastDataEN : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntfConfig0Register;

/**
 * @brief PWR_MGMT0 register.
 */
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

/**
 * @brief GYRO_CONFIG0 register.
 */
typedef union {

    struct {
        unsigned gyroOdr : 4;
        unsigned : 1;
        unsigned gyroFSSel : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmGyroConfig0Register;

/**
 * @brief ACCEL_CONFIG0 register.
 */
typedef union {

    struct {
        unsigned accelOdr : 4;
        unsigned : 1;
        unsigned accelFSSel : 3;
    } __attribute__((__packed__));
    uint8_t value;
} IcmAccelConfig0Register;

/**
 * @brief INT_CONFIG1 register.
 */
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

/**
 * @brief INT_SOURCE0 register.
 */
typedef union {

    struct {
        unsigned UIAgcRdyInt1EN : 1;
        unsigned FifoFullInt1EN : 1;
        unsigned FifoThsInt1EN : 1;
        unsigned UIDrdyInt1EN : 1;
        unsigned ResetDoneInt1EN : 1;
        unsigned PllRdyInt1EN : 1;
        unsigned UIFsyncInt1EN : 1;
        unsigned : 1;
    } __attribute__((__packed__));
    uint8_t value;
} IcmIntSource0Register;

/**
 * @brief SPI packet.
 */
typedef struct {
    unsigned int address : 7;
    unsigned int rw : 1;

    union {
        uint8_t data[15];
        uint8_t value;
    };
} __attribute__((__packed__)) IcmSpiPacket;

/**
 * @brief FIFO packet.
 */
typedef struct {
    uint64_t timestamp;
    IcmSensorRegisters registers;
} __attribute__((__packed__)) IcmFifoPacket;

/**
 * @brief ODR.
 */
typedef enum {
    IcmOdr32kHz = 0b0001,
    IcmOdr16kHz = 0b0010,
    IcmOdr8kHz = 0b0011,
    IcmOdr4kHz = 0b0100,
    IcmOdr2kHz = 0b0101,
    IcmOdr1kHz = 0b0110,
    IcmOdr200Hz = 0b0111,
    IcmOdr100Hz = 0b1000,
    IcmOdr50Hz = 0b1001,
    IcmOdr25Hz = 0b1010,
    IcmOdr12Hz = 0b1011,
    IcmOdr500Hz = 0b1111,
} IcmOdr;

/**
 * @brief Result.
 */
typedef enum {
    IcmResultOk,
    IcmResultError,
} IcmResult;

/**
 * @brief Data.
 */
typedef struct {
    uint64_t timestamp;
    float gyroscopeX;
    float gyroscopeY;
    float gyroscopeZ;
    float accelerometerX;
    float accelerometerY;
    float accelerometerZ;
    float temperature;
} IcmData;

/**
 * @brief Test result.
 */
typedef enum {
    IcmTestResultPassed,
    IcmTestResultInvalidID,
    IcmTestResultInterruptFailed,
} IcmTestResult;

/**
 * @brief Interface.
 */
typedef struct {
    const void(*initialise)(const IcmOdr odr);
    const void (*deinitialise)(void);
    const IcmResult(*getData)(IcmData * const data);
    const uint32_t(*bufferOverflow)(void);
} Icm;

//------------------------------------------------------------------------------
// Variable declarations

extern const SpiSettings icmSpiSettings;

//------------------------------------------------------------------------------
// Function declaration

float IcmOdrToFloat(const IcmOdr odr);
const char* IcmTestResultToString(const IcmTestResult result);

#endif

//------------------------------------------------------------------------------
// End of file
