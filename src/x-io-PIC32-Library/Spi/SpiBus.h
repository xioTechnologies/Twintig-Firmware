/**
 * @file SpiBus1.h
 * @author Seb Madgwick
 * @brief Manages access to the a bus.
 */

#ifndef SPI_BUS_H
#define SPI_BUS_H

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief SPI bus client. All structure members are private.
 */
typedef struct {
    GPIO_PIN csPin;
    void (*transferComplete)(void);
    void* data;
    size_t numberOfBytes;
} SpiBusClient;

#endif

//------------------------------------------------------------------------------
// End of file
