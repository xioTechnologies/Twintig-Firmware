/**
 * @file Icm.h
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
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
 * @brief Sample rate.
 */
typedef enum {
    IcmSampleRate12Hz = 12,
    IcmSampleRate25Hz = 25,
    IcmSampleRate50Hz = 50,
    IcmSampleRate100Hz = 100,
    IcmSampleRate200Hz = 200,
    IcmSampleRate400Hz = 400,
    IcmSampleRate800Hz = 800,
    IcmSampleRate1600Hz = 1600,
    IcmSampleRate3200Hz = 3200,
    IcmSampleRate6400Hz = 6400,
} IcmSampleRate;

/**
 * @brief Low-pass filter.
 */
typedef enum {
    IcmLowPassFilterBypass = 0,
    IcmLowPassFilter4 = 4,
    IcmLowPassFilter8 = 8,
    IcmLowPassFilter16 = 16,
    IcmLowPassFilter32 = 32,
    IcmLowPassFilter64 = 64,
    IcmLowPassFilter128 = 128,
} IcmLowPassFilter;

/**
 * @brief Settings.
 */
typedef struct {
    IcmSampleRate sampleRate;
    IcmLowPassFilter gyroscopeLowPassFilter;
    IcmLowPassFilter accelerometerLowPassFilter;
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
    void (*const initialise) (const IcmSettings * const settings);
    void (*const deinitialise) (void);
    IcmResult(*const getData)(IcmData * const data);
    uint32_t(*const bufferOverflow)(void);
} Icm;

//------------------------------------------------------------------------------
// Variable declarations

extern const SpiSettings icmSpiSettings;

//------------------------------------------------------------------------------
// Function declarations

const char* IcmTestResultToString(const IcmTestResult result);

#endif

//------------------------------------------------------------------------------
// End of file
