/**
 * @file Nvm.c
 * @author Seb Madgwick
 * @brief Non-Volatile Memory.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
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
// Definitions

/**
 * @brief Quotient size.
 */
#define QUOTIENT_SIZE (EEPROM_SIZE / 4)

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Variables

const Nvm nvmMain = {.i2c = &i2cBB1, .address = 0};
const Nvm nvmA = {.i2c = &i2c3, .address = 0};
const Nvm nvmB = {.i2c = &i2c3, .address = nvmA.address + QUOTIENT_SIZE};
const Nvm nvmC = {.i2c = &i2c3, .address = nvmB.address + QUOTIENT_SIZE};
const Nvm nvmD = {.i2c = &i2c3, .address = nvmC.address + QUOTIENT_SIZE};
const Nvm nvmE = {.i2c = &i2c2, .address = 0};
const Nvm nvmF = {.i2c = &i2c2, .address = nvmE.address + QUOTIENT_SIZE};
const Nvm nvmG = {.i2c = &i2c2, .address = nvmF.address + QUOTIENT_SIZE};
const Nvm nvmH = {.i2c = &i2c2, .address = nvmG.address + QUOTIENT_SIZE};
const Nvm nvmI = {.i2c = &i2c5, .address = 0};
const Nvm nvmJ = {.i2c = &i2c5, .address = nvmI.address + QUOTIENT_SIZE};
const Nvm nvmK = {.i2c = &i2c5, .address = nvmJ.address + QUOTIENT_SIZE};
const Nvm nvmL = {.i2c = &i2c5, .address = nvmK.address + QUOTIENT_SIZE};
const Nvm nvmM = {.i2c = &i2c1, .address = 0};
const Nvm nvmN = {.i2c = &i2c1, .address = nvmM.address + QUOTIENT_SIZE};
const Nvm nvmO = {.i2c = &i2c1, .address = nvmN.address + QUOTIENT_SIZE};
const Nvm nvmP = {.i2c = &i2c1, .address = nvmO.address + QUOTIENT_SIZE};
const Nvm nvmQ = {.i2c = &i2c4, .address = 0};
const Nvm nvmR = {.i2c = &i2c4, .address = nvmQ.address + QUOTIENT_SIZE};
const Nvm nvmS = {.i2c = &i2c4, .address = nvmR.address + QUOTIENT_SIZE};
const Nvm nvmT = {.i2c = &i2c4, .address = nvmS.address + QUOTIENT_SIZE};

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
