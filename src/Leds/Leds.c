/**
 * @file Leds.c
 * @author Seb Madgwick
 * @brief Driver for LEDs.
 */

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Leds.h"
#include "NeoPixels/NeoPixels.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises module. This function should be called once, on system
 * start up.
 */
void LedsInitialise(void) {
    NeoPixelsInitialise();
    GPIO_PinSet(ENABLE_PIN);
    GPIO_PinSet(ENABLE_CH1_PIN);
    GPIO_PinSet(ENABLE_CH2_PIN);
    GPIO_PinSet(ENABLE_CH3_PIN);
    GPIO_PinSet(ENABLE_CH4_PIN);
    GPIO_PinSet(ENABLE_CH5_PIN);
    static const NeoPixelsPixel pixels[] = {
        {.rgb = 0x040404},
        {.rgb = 0x040404},
        {.rgb = 0x040404},
        {.rgb = 0x040404},
    };
    NeoPixelsSet(pixels);
}

//------------------------------------------------------------------------------
// End of file
