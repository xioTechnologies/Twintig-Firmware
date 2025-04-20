/**
 * @file Icm.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM_H
#define ICM_H

//------------------------------------------------------------------------------
// Includes

#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

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

//------------------------------------------------------------------------------
// Function declaration

void IcmInitialise(const IcmOdr odr);
void IcmDeinitialise(void);
bool IcmGetData(IcmData * const data);

//------------------------------------------------------------------------------
// End of file

#endif
