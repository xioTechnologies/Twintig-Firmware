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
 * @brief Anti-alias filter.
 */
typedef struct {
    int delt;
    int deltsqr;
    int bitshift;
} IcmAaf;

/**
 * @brief FIFO packet.
 */
typedef struct {
    uint64_t ticks;
    IcmSensorRegisters registers;
} __attribute__((__packed__)) IcmFifoPacket;

/**
 * @brief Anti-aliasing.
 */
typedef enum {
    IcmAntiAliasingDisabled = 0,
    IcmAntiAliasing42Hz = 42,
    IcmAntiAliasing84Hz = 84,
    IcmAntiAliasing126Hz = 126,
    IcmAntiAliasing170Hz = 170,
    IcmAntiAliasing213Hz = 213,
    IcmAntiAliasing258Hz = 258,
    IcmAntiAliasing303Hz = 303,
    IcmAntiAliasing348Hz = 348,
    IcmAntiAliasing394Hz = 394,
    IcmAntiAliasing441Hz = 441,
    IcmAntiAliasing488Hz = 488,
    IcmAntiAliasing536Hz = 536,
    IcmAntiAliasing585Hz = 585,
    IcmAntiAliasing634Hz = 634,
    IcmAntiAliasing684Hz = 684,
    IcmAntiAliasing734Hz = 734,
    IcmAntiAliasing785Hz = 785,
    IcmAntiAliasing837Hz = 837,
    IcmAntiAliasing890Hz = 890,
    IcmAntiAliasing943Hz = 943,
    IcmAntiAliasing997Hz = 997,
    IcmAntiAliasing1051Hz = 1051,
    IcmAntiAliasing1107Hz = 1107,
    IcmAntiAliasing1163Hz = 1163,
    IcmAntiAliasing1220Hz = 1220,
    IcmAntiAliasing1277Hz = 1277,
    IcmAntiAliasing1336Hz = 1336,
    IcmAntiAliasing1395Hz = 1395,
    IcmAntiAliasing1454Hz = 1454,
    IcmAntiAliasing1515Hz = 1515,
    IcmAntiAliasing1577Hz = 1577,
    IcmAntiAliasing1639Hz = 1639,
    IcmAntiAliasing1702Hz = 1702,
    IcmAntiAliasing1766Hz = 1766,
    IcmAntiAliasing1830Hz = 1830,
    IcmAntiAliasing1896Hz = 1896,
    IcmAntiAliasing1962Hz = 1962,
    IcmAntiAliasing2029Hz = 2029,
    IcmAntiAliasing2097Hz = 2097,
    IcmAntiAliasing2166Hz = 2166,
    IcmAntiAliasing2235Hz = 2235,
    IcmAntiAliasing2306Hz = 2306,
    IcmAntiAliasing2377Hz = 2377,
    IcmAntiAliasing2449Hz = 2449,
    IcmAntiAliasing2522Hz = 2522,
    IcmAntiAliasing2596Hz = 2596,
    IcmAntiAliasing2671Hz = 2671,
    IcmAntiAliasing2746Hz = 2746,
    IcmAntiAliasing2823Hz = 2823,
    IcmAntiAliasing2900Hz = 2900,
    IcmAntiAliasing2978Hz = 2978,
    IcmAntiAliasing3057Hz = 3057,
    IcmAntiAliasing3137Hz = 3137,
    IcmAntiAliasing3217Hz = 3217,
    IcmAntiAliasing3299Hz = 3299,
    IcmAntiAliasing3381Hz = 3381,
    IcmAntiAliasing3464Hz = 3464,
    IcmAntiAliasing3548Hz = 3548,
    IcmAntiAliasing3633Hz = 3633,
    IcmAntiAliasing3718Hz = 3718,
    IcmAntiAliasing3805Hz = 3805,
    IcmAntiAliasing3892Hz = 3892,
    IcmAntiAliasing3979Hz = 3979,
} IcmAntiAliasing;

/**
 * @brief Sample rate.
 */
typedef enum {
    IcmSampleRate32kHz = 32000,
    IcmSampleRate16kHz = 16000,
    IcmSampleRate8kHz = 8000,
    IcmSampleRate4kHz = 4000,
    IcmSampleRate2kHz = 2000,
    IcmSampleRate1kHz = 1000,
    IcmSampleRate500Hz = 500,
    IcmSampleRate200Hz = 200,
    IcmSampleRate100Hz = 100,
    IcmSampleRate50Hz = 50,
    IcmSampleRate25Hz = 25,
    IcmSampleRate12Hz = 12,
} IcmSampleRate;

/**
 * @brief Settings.
 */
typedef struct {
    bool gyroscopeNotchFilterEnabled;
    IcmAntiAliasing gyroscopeAntiAliasing;
    IcmAntiAliasing accelerometerAntiAliasing;
    IcmSampleRate sampleRate;
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
    uint64_t ticks;
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

//------------------------------------------------------------------------------
// Function declarations

IcmAaf IcmAntiAliasingToAaf(const IcmAntiAliasing antiAliasing);
int IcmSampleRateToOdr(const IcmSampleRate sampleRate);
const char* IcmTestResultToString(const IcmTestResult result);

#endif

//------------------------------------------------------------------------------
// End of file
