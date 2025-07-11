/**
 * @file Interfaces.c
 * @author Seb Madgwick
 * @brief Interfaces.
 */

//------------------------------------------------------------------------------
// Includes

#include "Context.h"
#include "Interfaces.h"
#include "Send/Send.h"
#include "Serial/Serial.h"
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
    return UsbCdcRead(destination, numberOfBytes);
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 */
void InterfacesUsbWrite(const void* const data, const size_t numberOfBytes, void* const context) {
    const Context * const context_ = context;
    SendResponseUsb(context_->send, data, numberOfBytes);
}

/**
 * @brief Reads data from the read buffer.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 * @return Number of bytes read.
 */
size_t InterfacesSerialRead(void* const destination, size_t numberOfBytes, void* const context) {
    return SerialRead(destination, numberOfBytes);
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param context Context.
 */
void InterfacesSerialWrite(const void* const data, const size_t numberOfBytes, void* const context) {
    const Context * const context_ = context;
    SendResponseSerial(context_->send, data, numberOfBytes);
}

//------------------------------------------------------------------------------
// End of file
