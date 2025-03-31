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

//------------------------------------------------------------------------------
// Function declarations

void NvmRead(void* const destination, size_t numberOfBytes, void* const context);
void NvmWrite(const void* const data, const size_t numberOfBytes, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
