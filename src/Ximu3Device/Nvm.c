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

const Nvm nvm0 = {.i2c = &i2cBB1, .address = 0};
const Nvm nvm1 = {.i2c = &i2c3, .address = 0};
const Nvm nvm2 = {.i2c = &i2c3, .address = 1024};
const Nvm nvm3 = {.i2c = &i2c3, .address = 2048};
const Nvm nvm4 = {.i2c = &i2c3, .address = 3072};
const Nvm nvm5 = {.i2c = &i2c2, .address = 0};
const Nvm nvm6 = {.i2c = &i2c2, .address = 1024};
const Nvm nvm7 = {.i2c = &i2c2, .address = 2048};
const Nvm nvm8 = {.i2c = &i2c2, .address = 3072};
const Nvm nvm9 = {.i2c = &i2c5, .address = 0};
const Nvm nvm10 = {.i2c = &i2c5, .address = 1024};
const Nvm nvm11 = {.i2c = &i2c5, .address = 2048};
const Nvm nvm12 = {.i2c = &i2c5, .address = 3072};
const Nvm nvm13 = {.i2c = &i2c1, .address = 0};
const Nvm nvm14 = {.i2c = &i2c1, .address = 1024};
const Nvm nvm15 = {.i2c = &i2c1, .address = 2048};
const Nvm nvm16 = {.i2c = &i2c1, .address = 3072};
const Nvm nvm17 = {.i2c = &i2c4, .address = 0};
const Nvm nvm18 = {.i2c = &i2c4, .address = 1024};
const Nvm nvm19 = {.i2c = &i2c4, .address = 2048};
const Nvm nvm20 = {.i2c = &i2c4, .address = 3072};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Reads from NVM.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 */
void NvmRead(void* const destination, size_t numberOfBytes, void* const context) {
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
    EepromWrite(context_->nvm->i2c, context_->nvm->address, data, numberOfBytes);
}

//------------------------------------------------------------------------------
// End of file
