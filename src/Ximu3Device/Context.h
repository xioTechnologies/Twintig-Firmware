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
#include "Nvm.h"
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
    const char* const defaultName;
    Ximu3Settings * const settings;
    bool nvmBlank;
    bool factoryMode;
    uint64_t applyTimeout;
    const Nvm * const nvm;
    Imu * const imu;
    Send * const send;
} Context;

#endif

//------------------------------------------------------------------------------
// End of file
