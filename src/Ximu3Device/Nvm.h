/**
 * @file Nvm.h
 * @author Seb Madgwick
 * @brief Non-Volatile Memory.
 */

#ifndef NVM_H
#define NVM_H

//------------------------------------------------------------------------------
// Includes

#include <stddef.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Function declarations

void NvmRead(const uint32_t address, void* const destination, size_t numberOfBytes, void* const context);
void NvmWrite(const uint32_t address, const void* const data, const size_t numberOfBytes, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
