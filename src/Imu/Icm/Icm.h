/**
 * @file Icm.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM_H
#define ICM_H

//------------------------------------------------------------------------------
// Includes

#include "IcmRegisters.h"
#include "Spi/Spi.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

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
    IcmTestResultInvalidId,
    IcmTestResultInterruptFailed,
} IcmTestResult;

/**
 * @brief ICM interface.
 */
typedef struct {
    void(*initialise)(const IcmOdr odr);
    void (*deinitialise)(void);
    IcmResult(*getData)(IcmData * const data);
    uint32_t(*bufferOverflow)(void);
} Icm;

//------------------------------------------------------------------------------
// Variable declarations

extern const SpiSettings icmSpiSettings;

//------------------------------------------------------------------------------
// Function declaration

const char* IcmTestResultToString(const IcmTestResult result);

#endif

//------------------------------------------------------------------------------
// End of file
