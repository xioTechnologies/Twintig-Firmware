/**
 * @file Interfaces.h
 * @author Seb Madgwick
 * @brief Interfaces.
 */

#ifndef INTERFACES_H
#define INTERFACES_H

//------------------------------------------------------------------------------
// Includes

#include <stddef.h>

//------------------------------------------------------------------------------
// Function declarations

size_t InterfacesUsbRead(void* const destination, size_t numberOfBytes, void* const context);
void InterfacesUsbWrite(const void* const data, const size_t numberOfBytes, void* const context);
size_t InterfacesSerialRead(void* const destination, size_t numberOfBytes, void* const context);
void InterfacesSerialWrite(const void* const data, const size_t numberOfBytes, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
