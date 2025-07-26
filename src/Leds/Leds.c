/**
 * @file Leds.c
 * @author Seb Madgwick
 * @brief LEDs driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Leds.h"
#include "NeoPixels/NeoPixels1.h"
#include "Timer/TimerScheduler.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises module. This function should be called once, on system
 * start up.
 */
void LedsInitialise(void) {
    GPIO_PinSet(ENABLE_PIN);
    GPIO_PinSet(ENABLE_CH1_PIN);
    GPIO_PinSet(ENABLE_CH2_PIN);
    GPIO_PinSet(ENABLE_CH3_PIN);
    GPIO_PinSet(ENABLE_CH4_PIN);
    GPIO_PinSet(ENABLE_CH5_PIN);
    neoPixels1Pixels[0].rgb = 0x040404;
    neoPixels1Pixels[1].rgb = 0x040404;
    neoPixels1Pixels[2].rgb = 0x040404;
    neoPixels1Pixels[3].rgb = 0x040404;
    NeoPixels1Update();
}

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 */
void LedsTasks(void) {
    if (TIMER_SCHEDULER_POLL(0.1f) == false) {
        return;
    }
    static int counter;
    if ((counter++ & 1) != 0) {
        neoPixels1Pixels[0].rgb = 0x040404;
        neoPixels1Pixels[1].rgb = 0x040404;
        neoPixels1Pixels[2].rgb = 0x040404;
        neoPixels1Pixels[3].rgb = 0x040404;
    } else {
        neoPixels1Pixels[0].rgb = 0x000000;
        neoPixels1Pixels[1].rgb = 0x000000;
        neoPixels1Pixels[2].rgb = 0x000000;
        neoPixels1Pixels[3].rgb = 0x000000;
    }
    NeoPixels1Update();
}

//------------------------------------------------------------------------------
// End of file
