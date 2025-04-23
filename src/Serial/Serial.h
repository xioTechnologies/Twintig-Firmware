/**
 * @file Serial.h
 * @author Seb Madgwick
 * @brief Serial driver.
 */

#ifndef SERIAL_H
#define SERIAL_H

//------------------------------------------------------------------------------
// Includes

#include "Fifo.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Serial settings.
 */
typedef struct {
    bool enabled;
    uint32_t baudRate;
    bool rtsCtsEnabled;
} SerialSettings;

//------------------------------------------------------------------------------
// Function prototypes

void SerialSetSettings(const SerialSettings * const settings_);
size_t SerialRead(void* const destination, size_t numberOfBytes);
size_t SerialGetWriteAvailable(void);
FifoResult SerialWrite(const void* const data, const size_t numberOfBytes);

#endif

//------------------------------------------------------------------------------
// End of file
