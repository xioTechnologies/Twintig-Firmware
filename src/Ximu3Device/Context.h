/**
 * @file Context.h
 * @author Seb Madgwick
 * @brief Context.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

//------------------------------------------------------------------------------
// Includes

#include <stdbool.h>
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
} Context;

#endif

//------------------------------------------------------------------------------
// End of file
