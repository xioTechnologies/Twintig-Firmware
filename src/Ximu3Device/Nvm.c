/**
 * @file Nvm.c
 * @author Seb Madgwick
 * @brief Non-Volatile Memory.
 */

//------------------------------------------------------------------------------
// Includes

#include "Nvm.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Reads from NVM.
 * @param address Address.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @param context Context. 
 */
void NvmRead(const uint32_t address, void* const destination, size_t numberOfBytes, void* const context) {
}

/**
 * @brief Writes to NVM.
 * @param address Address.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context. 
 */
void NvmWrite(const uint32_t address, const void* const data, const size_t numberOfBytes, void* const context) {
}

//------------------------------------------------------------------------------
// End of file
