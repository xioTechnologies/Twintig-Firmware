/**
 * @file Context.h
 * @author Seb Madgwick
 * @brief Context.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

//------------------------------------------------------------------------------
// Includes

#include "Imu/Imu.h"
#include "Led/Led.h"
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
    Ximu3Settings * const settings;
    bool nvmBlank;
    bool factoryMode;
    uint64_t applyTimeout;
    const Nvm * const nvm;
    Send * const send;
    Led * const led;
    const bool isMain;
    Imu * const imu; // NULL if unused
    const char* const defaultName; // NULL if unused
} Context;

#endif

//------------------------------------------------------------------------------
// End of file
