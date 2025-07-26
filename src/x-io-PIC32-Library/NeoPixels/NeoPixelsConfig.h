/**
 * @file NeoPixelsConfig.h
 * @author Seb Madgwick
 * @brief NeoPixels driver.
 */

#ifndef NEOPIXELS_CONFIG_H
#define NEOPIXELS_CONFIG_H

//------------------------------------------------------------------------------
// Includes

#include "Spi/Spi1DmaTx.h"

//------------------------------------------------------------------------------
// Definitions

#define NEOPIXELS_1_HAL_NUMBER_OF_PIXELS            (4)
#define NEOPIXELS_1_SPI_BUS_TRANSFER                Spi1DmaTxTransfer
#define NEOPIXELS_1_SPI_BUS_TRANSFER_IN_PROGRESS    Spi1DmaTxTransferInProgress

#endif

//------------------------------------------------------------------------------
// End of file
