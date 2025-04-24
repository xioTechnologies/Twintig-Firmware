/**
 * @file Nvm.c
 * @author Seb Madgwick
 * @brief Non-Volatile Memory.
 */

//------------------------------------------------------------------------------
// Includes

#include "Context.h"
#include "Eeprom/Eeprom.h"
#include "Nvm.h"

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
    EepromRead(context_->i2c, context_->address, destination, numberOfBytes);
}

/**
 * @brief Writes to NVM.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 */
void NvmWrite(const void* const data, const size_t numberOfBytes, void* const context) {
    const Context * const context_ = context;
    EepromWrite(context_->i2c, context_->address, data, numberOfBytes);
}

//------------------------------------------------------------------------------
// End of file
