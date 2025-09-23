/**
 * @file Serial.c
 * @author Seb Madgwick
 * @brief Serial driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "Serial.h"
#include "Uart/Uart1DmaTx.h"

//------------------------------------------------------------------------------
// Function declarations

static void WriteComplete(void);

//------------------------------------------------------------------------------
// Variables

static SerialSettings settings;
static uint8_t fifoData[20 * 4096];
static Fifo fifo = {.data = fifoData, .dataSize = sizeof (fifoData)};

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
        Uart1DmaTxInitialise(&uartSettings);
    } else {
        Uart1DmaTxDeinitialise();
        FifoClear(&fifo);
    }
}

/**
 * @brief Returns true if enabled.
 * @return True if enabled.
 */
bool SerialEnabled(void) {
    return settings.enabled;
}

/**
 * @brief Reads data from the read buffer.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @return Number of bytes read.
 */
size_t SerialRead(void* const destination, size_t numberOfBytes) {
    return Uart1DmaTxRead(destination, numberOfBytes);
}

/**
 * @brief Returns the space available in the write buffer.
 * @return Space available in the write buffer.
 */
size_t SerialAvailableWrite(void) {
    return FifoAvailableWrite(&fifo);
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
FifoResult SerialWrite(const void* const data, const size_t numberOfBytes) {
    const FifoResult result = FifoWrite(&fifo, data, numberOfBytes);
    if (Uart1DmaTxWriteInProgress() == false) {
        WriteComplete();
    }
    return result;
}

/**
 * @brief Write complete callback.
 */
static void WriteComplete(void) {
    static __attribute__((coherent)) uint8_t data[2048];
    const size_t numberOfBytes = FifoRead(&fifo, data, sizeof (data));
    if (numberOfBytes > 0) {
        Uart1DmaTxWrite(data, numberOfBytes, WriteComplete);
    }
}

//------------------------------------------------------------------------------
// End of file
