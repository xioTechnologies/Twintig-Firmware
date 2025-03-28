/**
 * @file NeoPixelsHal.h
 * @author Seb Madgwick
 * @brief NeoPixels driver Hardware Abstraction Layer (HAL).
 */

#ifndef NEOPIXELS_HAL_H
#define NEOPIXELS_HAL_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Spi/Spi.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Number of NeoPixels.
 */
#define NEOPIXELS_HAL_NUMBER_OF_PIXELS 1

//------------------------------------------------------------------------------
// Inline functions

/**
 * @brief Initialises the module.
 * @param settings Settings.
 */
static inline __attribute__((always_inline)) void NeoPixelsHalSpiInitialise(const SpiSettings * const settings) {
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.ENHBUF = 1;
    SPI1BRG = SpiCalculateSpixbrg(settings->clockFrequency);
    SPI1CONbits.ON = 1;
}

/**
 * @brief Transfers data. The data will be overwritten with the received data.
 * This function must not be called while a transfer is in progress. The
 * callback function will be called from within an interrupt once the transfer
 * is complete.
 * @param csPin CS pin.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param transferComplete Transfer complete callback function.
 */
static inline __attribute__((always_inline)) void NeoPixelsHalSpiTransfer(const GPIO_PIN csPin, void* const data, const size_t numberOfBytes, void (*transferComplete)(void)) {
    for (int index = 0; index < numberOfBytes; index++) {
        while (SPI1STATbits.SPITBF == 1); // wait while transmit buffer full
        SPI1BUF = ((uint8_t*) data)[index];
    }
}

/**
 * @brief Returns true while the transfer is in progress.
 * @return True while the transfer is in progress.
 */
static inline __attribute__((always_inline)) bool NeoPixelsHalSpiTransferInProgress(void) {
    return false;
}

#endif

//------------------------------------------------------------------------------
// End of file
