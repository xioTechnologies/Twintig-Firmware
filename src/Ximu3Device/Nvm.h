/**
 * @file Nvm.h
 * @author Seb Madgwick
 * @brief Non-Volatile Memory.
 */

#ifndef NVM_H
#define NVM_H

//------------------------------------------------------------------------------
// Includes

#include "I2C/I2C.h"
#include <stddef.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief NVM structure.
 */
typedef struct {
    const I2C * const i2c;
    const uint16_t address;
} Nvm;

//------------------------------------------------------------------------------
// Variable declarations

extern const Nvm nvm0;
extern const Nvm nvm1;
extern const Nvm nvm1;
extern const Nvm nvm2;
extern const Nvm nvm3;
extern const Nvm nvm4;
extern const Nvm nvm5;
extern const Nvm nvm6;
extern const Nvm nvm7;
extern const Nvm nvm8;
extern const Nvm nvm9;
extern const Nvm nvm10;
extern const Nvm nvm11;
extern const Nvm nvm12;
extern const Nvm nvm13;
extern const Nvm nvm14;
extern const Nvm nvm15;
extern const Nvm nvm16;
extern const Nvm nvm17;
extern const Nvm nvm18;
extern const Nvm nvm19;
extern const Nvm nvm20;

//------------------------------------------------------------------------------
// Function declarations

void NvmRead(void* const destination, size_t numberOfBytes, void* const context);
void NvmWrite(const void* const data, const size_t numberOfBytes, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
