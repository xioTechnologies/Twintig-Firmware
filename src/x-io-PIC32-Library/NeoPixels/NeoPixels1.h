/**
 * @file NeoPixels1.h
 * @author Seb Madgwick
 * @brief NeoPixels driver.
 */

#ifndef NEOPIXELS_1_H
#define NEOPIXELS_1_H

//------------------------------------------------------------------------------
// Includes

#include "NeoPixels.h"
#include "NeoPixelsConfig.h"
#include <stddef.h>

//------------------------------------------------------------------------------
// Variable declarations

extern NeoPixelsPixel neoPixels1Pixels[NEOPIXELS_1_HAL_NUMBER_OF_PIXELS];
extern void* const neoPixels1SpiData;
extern const size_t neoPixels1SpiNumberOfBytes;

//------------------------------------------------------------------------------
// Function declarations

void NeoPixels1Update(void);

#endif

//------------------------------------------------------------------------------
// End of file
