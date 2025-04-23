/**
 * @file Serial.c
 * @author Seb Madgwick
 * @brief Serial driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "Serial.h"
#include "Uart/Uart1.h"

//------------------------------------------------------------------------------
// Variables

static SerialSettings settings;

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Sets the settings.
 * @param settings Settings.
 */
void SerialSetSettings(const SerialSettings * const settings_) {
    settings = *settings_;
    if (settings.enabled) {
        UartSettings uartSettings = uartSettingsDefault;
        uartSettings.baudRate = settings.baudRate;
        uartSettings.rtsCtsEnabled = settings.rtsCtsEnabled;
        Uart1Initialise(&uartSettings);
    } else {
        Uart1Deinitialise();
    }
}

/**
 * @brief Reads data from the read buffer.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @return Number of bytes read.
 */
size_t SerialRead(void* const destination, size_t numberOfBytes) {
    return Uart1Read(destination, numberOfBytes);
}

/**
 * @brief Returns the space available in the write buffer.
 * @return Space available in the write buffer.
 */
size_t SerialGetWriteAvailable(void) {
    return Uart1GetWriteAvailable();
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
FifoResult SerialWrite(const void* const data, const size_t numberOfBytes) {
    return Uart1Write(data, numberOfBytes);
}

//------------------------------------------------------------------------------
// End of file
