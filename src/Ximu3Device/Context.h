/**
 * @file Context.h
 * @author Seb Madgwick
 * @brief Context.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

//------------------------------------------------------------------------------
// Includes

#include "Haptic/Haptic.h"
#include "Imu/Imu.h"
#include "Led/Led.h"
#include "Nvm.h"
#include "Send/Send.h"
#include "Serial/Serial.h"
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
    Send * const send;
    Led * const led;
    void(*const serialSetSettings) (const SerialSettings * const settings); // NULL if unused
    HapticResult(*const hapticPlay)(const int effect); // NULL if unused
    Imu * const imu; // NULL if unused
} Context;

#endif

//------------------------------------------------------------------------------
// End of file
