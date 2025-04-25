/**
 * @file Icm.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM_H
#define ICM_H

//------------------------------------------------------------------------------
// Includes

#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Result.
 */
typedef enum {
    IcmResultOK,
    IcmResultError,
} IcmResult;

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

extern const Icm icm1;

//------------------------------------------------------------------------------
// Function declaration

void IcmInitialise(const IcmOdr odr);
void IcmDeinitialise(void);
IcmResult IcmGetData(IcmData * const data);
uint32_t IcmBufferOverflow(void);
float IcmOdrToFloat(const IcmOdr odr);
IcmTestResult IcmTest(void);
const char* IcmTestResultToString(const IcmTestResult result);

//------------------------------------------------------------------------------
// End of file

#endif
