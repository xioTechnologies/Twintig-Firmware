/**
 * @file SpiBus.h
 * @author Seb Madgwick
 * @brief SPI bus.
 */

#ifndef SPI_BUS_H
#define SPI_BUS_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief SPI bus client. All structure members are private.
 */
typedef struct {
    GPIO_PIN csPin;
    void* data;
    size_t numberOfBytes;
    void (*transferComplete)(void);
    bool inProgress;
} SpiBusClient;

#endif

//------------------------------------------------------------------------------
// End of file
