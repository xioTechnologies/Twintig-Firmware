/**
 * @file Imu.h
 * @author Seb Madgwick
 * @brief ICM-42688-P IMU driver.
 */

#ifndef IMU_H
#define IMU_H

//------------------------------------------------------------------------------
// Includes

#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

typedef enum {
    ImuOdr32kHz = 0b0001,
    ImuOdr16kHz = 0b0010,
    ImuOdr8kHz = 0b0011,
    ImuOdr4kHz = 0b0100,
    ImuOdr2kHz = 0b0101,
    ImuOdr1kHz = 0b0110,
    ImuOdr200Hz = 0b0111,
    ImuOdr100Hz = 0b1000,
    ImuOdr50Hz = 0b1001,
    ImuOdr25Hz = 0b1010,
} ImuOdr;

typedef struct {
    uint64_t timestamp;
    float gyroscopeX;
    float gyroscopeY;
    float gyroscopeZ;
    float accelerometerX;
    float accelerometerY;
    float accelerometerZ;
    float temperature;
} ImuData;

//------------------------------------------------------------------------------
// Function declaration

void ImuInitialise(const ImuOdr odr);
void ImuDeinitialise(void);
void ImuSetDataReadyCallback(void (*dataReady_)(const ImuData * const data));
void ImuRead(void);

//------------------------------------------------------------------------------
// End of file

#endif
