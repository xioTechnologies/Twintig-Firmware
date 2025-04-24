/**
 * @file Context.h
 * @author Seb Madgwick
 * @brief Context.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

//------------------------------------------------------------------------------
// Includes

#include "I2C/I2C.h"
#include "Imu/Imu.h"
#include "Send/Send.h"
#include <stdbool.h>
#include <stdint.h>
#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Context.
 */
typedef struct {
    Ximu3Settings * const settings;
    bool nvmBlank;
    bool factoryMode;
    uint64_t applyTimeout;
    const I2C * const i2c;
    const uint16_t address;
    Imu * const imu;
    Send * const send;
} Context;

#endif

//------------------------------------------------------------------------------
// End of file
