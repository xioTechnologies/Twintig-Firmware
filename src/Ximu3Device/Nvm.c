/**
 * @file Nvm.c
 * @author Seb Madgwick
 * @brief Non-Volatile Memory.
 */

//------------------------------------------------------------------------------
// Includes

#include "Context.h"
#include "Eeprom/Eeprom.h"
#include "I2C/I2C1.h"
#include "I2C/I2C2.h"
#include "I2C/I2C3.h"
#include "I2C/I2C4.h"
#include "I2C/I2C5.h"
#include "I2C/I2CBB1.h"
#include "Nvm.h"

//------------------------------------------------------------------------------
// Variables

const Nvm nvmMain = {.i2c = &i2cBB1, .address = 0};
const Nvm nvmA = {.i2c = &i2c3, .address = 0};
const Nvm nvmB = {.i2c = &i2c3, .address = 1024};
const Nvm nvmC = {.i2c = &i2c3, .address = 2048};
const Nvm nvmD = {.i2c = &i2c3, .address = 3072};
const Nvm nvmE = {.i2c = &i2c2, .address = 0};
const Nvm nvmF = {.i2c = &i2c2, .address = 1024};
const Nvm nvmG = {.i2c = &i2c2, .address = 2048};
const Nvm nvmH = {.i2c = &i2c2, .address = 3072};
const Nvm nvmI = {.i2c = &i2c5, .address = 0};
const Nvm nvmJ = {.i2c = &i2c5, .address = 1024};
const Nvm nvmK = {.i2c = &i2c5, .address = 2048};
const Nvm nvmL = {.i2c = &i2c5, .address = 3072};
const Nvm nvmM = {.i2c = &i2c1, .address = 0};
const Nvm nvmN = {.i2c = &i2c1, .address = 1024};
const Nvm nvmO = {.i2c = &i2c1, .address = 2048};
const Nvm nvmP = {.i2c = &i2c1, .address = 3072};
const Nvm nvmQ = {.i2c = &i2c4, .address = 0};
const Nvm nvmR = {.i2c = &i2c4, .address = 1024};
const Nvm nvmS = {.i2c = &i2c4, .address = 2048};
const Nvm nvmT = {.i2c = &i2c4, .address = 3072};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Reads from NVM.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 */
void NvmRead(void* const destination, const size_t numberOfBytes, void* const context) {
    const Context * const context_ = context;
    EepromRead(context_->nvm->i2c, context_->nvm->address, destination, numberOfBytes);
}

/**
 * @brief Writes to NVM.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 */
void NvmWrite(const void* const data, const size_t numberOfBytes, void* const context) {
    const Context * const context_ = context;
    EepromUpdate(context_->nvm->i2c, context_->nvm->address, data, numberOfBytes);
}

//------------------------------------------------------------------------------
// End of file
