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
#include <stdbool.h>
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
 * @brief Anti-alias filter.
 */
typedef struct {
    int delt;
    int deltsqr;
    int bitshift;
} IcmAaf;

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
 * @brief Settings.
 */
typedef struct {
    bool nfDisable;
    IcmAaf aaf;
    bool aafDisable;
    IcmOdr odr;
} IcmSettings;

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
    void(*initialise)(const IcmSettings * const settings);
    void (*deinitialise)(void);
    IcmResult(*getData)(IcmData * const data);
    uint32_t(*bufferOverflow)(void);
} Icm;

//------------------------------------------------------------------------------
// Variable declarations

extern const SpiSettings icmSpiSettings;

extern const IcmAaf icmAaf42Hz;
extern const IcmAaf icmAaf84Hz;
extern const IcmAaf icmAaf126Hz;
extern const IcmAaf icmAaf170Hz;
extern const IcmAaf icmAaf213Hz;
extern const IcmAaf icmAaf258Hz;
extern const IcmAaf icmAaf303Hz;
extern const IcmAaf icmAaf348Hz;
extern const IcmAaf icmAaf394Hz;
extern const IcmAaf icmAaf441Hz;
extern const IcmAaf icmAaf488Hz;
extern const IcmAaf icmAaf536Hz;
extern const IcmAaf icmAaf585Hz;
extern const IcmAaf icmAaf634Hz;
extern const IcmAaf icmAaf684Hz;
extern const IcmAaf icmAaf734Hz;
extern const IcmAaf icmAaf785Hz;
extern const IcmAaf icmAaf837Hz;
extern const IcmAaf icmAaf890Hz;
extern const IcmAaf icmAaf943Hz;
extern const IcmAaf icmAaf997Hz;
extern const IcmAaf icmAaf1051Hz;
extern const IcmAaf icmAaf1107Hz;
extern const IcmAaf icmAaf1163Hz;
extern const IcmAaf icmAaf1220Hz;
extern const IcmAaf icmAaf1277Hz;
extern const IcmAaf icmAaf1336Hz;
extern const IcmAaf icmAaf1395Hz;
extern const IcmAaf icmAaf1454Hz;
extern const IcmAaf icmAaf1515Hz;
extern const IcmAaf icmAaf1577Hz;
extern const IcmAaf icmAaf1639Hz;
extern const IcmAaf icmAaf1702Hz;
extern const IcmAaf icmAaf1766Hz;
extern const IcmAaf icmAaf1830Hz;
extern const IcmAaf icmAaf1896Hz;
extern const IcmAaf icmAaf1962Hz;
extern const IcmAaf icmAaf2029Hz;
extern const IcmAaf icmAaf2097Hz;
extern const IcmAaf icmAaf2166Hz;
extern const IcmAaf icmAaf2235Hz;
extern const IcmAaf icmAaf2306Hz;
extern const IcmAaf icmAaf2377Hz;
extern const IcmAaf icmAaf2449Hz;
extern const IcmAaf icmAaf2522Hz;
extern const IcmAaf icmAaf2596Hz;
extern const IcmAaf icmAaf2671Hz;
extern const IcmAaf icmAaf2746Hz;
extern const IcmAaf icmAaf2823Hz;
extern const IcmAaf icmAaf2900Hz;
extern const IcmAaf icmAaf2978Hz;
extern const IcmAaf icmAaf3057Hz;
extern const IcmAaf icmAaf3137Hz;
extern const IcmAaf icmAaf3217Hz;
extern const IcmAaf icmAaf3299Hz;
extern const IcmAaf icmAaf3381Hz;
extern const IcmAaf icmAaf3464Hz;
extern const IcmAaf icmAaf3548Hz;
extern const IcmAaf icmAaf3633Hz;
extern const IcmAaf icmAaf3718Hz;
extern const IcmAaf icmAaf3805Hz;
extern const IcmAaf icmAaf3892Hz;
extern const IcmAaf icmAaf3979Hz;

//------------------------------------------------------------------------------
// Function declaration

const char* IcmTestResultToString(const IcmTestResult result);

#endif

//------------------------------------------------------------------------------
// End of file
