/**
 * @file Interfaces.c
 * @author Seb Madgwick
 * @brief Interfaces.
 */

//------------------------------------------------------------------------------
// Includes

#include "Interfaces.h"
#include "Uart/Uart1.h"
#include "Usb/UsbCdc.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Reads data from the read buffer.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @param context Context. 
 * @return Number of bytes read.
 */
size_t InterfacesUsbRead(void* const destination, size_t numberOfBytes, void* const context) {
    return UsbCdcRead(destination, numberOfBytes); // TODO: use context for MUX
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context. 
 */
void InterfacesUsbWrite(const void* const data, const size_t numberOfBytes, void* const context) {
    UsbCdcWrite(data, numberOfBytes); // TODO: use context for MUX
}

/**
 * @brief Reads data from the read buffer.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @param context Context. 
 * @return Number of bytes read.
 */
size_t InterfacesSerialRead(void* const destination, size_t numberOfBytes, void* const context) {
    return Uart1Read(destination, numberOfBytes); // TODO: use context for MUX
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context. 
 */
void InterfacesSerialWrite(const void* const data, const size_t numberOfBytes, void* const context) {
    Uart1Write(data, numberOfBytes); // TODO: use context for MUX
}

//------------------------------------------------------------------------------
// End of file
