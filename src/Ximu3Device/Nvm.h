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
#include <stdint.h>

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

extern const Nvm nvmMain;
extern const Nvm nvmA;
extern const Nvm nvmB;
extern const Nvm nvmC;
extern const Nvm nvmD;
extern const Nvm nvmE;
extern const Nvm nvmF;
extern const Nvm nvmG;
extern const Nvm nvmH;
extern const Nvm nvmI;
extern const Nvm nvmJ;
extern const Nvm nvmK;
extern const Nvm nvmL;
extern const Nvm nvmM;
extern const Nvm nvmN;
extern const Nvm nvmO;
extern const Nvm nvmP;
extern const Nvm nvmQ;
extern const Nvm nvmR;
extern const Nvm nvmS;
extern const Nvm nvmT;

//------------------------------------------------------------------------------
// Function declarations

void NvmRead(void* const destination, const size_t numberOfBytes, void* const context);
void NvmWrite(const void* const data, const size_t numberOfBytes, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
