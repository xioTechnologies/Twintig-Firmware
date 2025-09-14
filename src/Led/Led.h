/**
 * @file Led.h
 * @author Seb Madgwick
 * @brief LED driver.
 */

#ifndef LED_H
#define LED_H

//------------------------------------------------------------------------------
// Includes

#include "NeoPixels/NeoPixels.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Blink queue length.
 */
#define LED_BLINK_QUEUE_LENGTH (4)

/**
 * @brief Colour.
 */
typedef union {

    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        unsigned : 8;
    } __attribute__((__packed__));
    uint32_t rgb;
} LedColour;

/**
 * @brief Mode.
 */
typedef enum {
    LedModeNormal,
    LedModeFlashing,
} LedMode;

/**
 * @brief Result.
 */
typedef enum {
    LedResultOk,
    LedResultError,
} LedResult;

/**
 * @brief LED structure. All structure members are private.
 */
typedef struct {
    NeoPixelsPixel * const pixel;
    LedColour colour;
    LedMode mode;
    LedColour blinkQueue[LED_BLINK_QUEUE_LENGTH];
    uint64_t strobeTimeout;
    bool overrideEnabled;
    LedColour overrideColour;
} Led;

//------------------------------------------------------------------------------
// Variable declarations

extern const LedColour ledColourRed;
extern const LedColour ledColourOrange;
extern const LedColour ledColourYellow;
extern const LedColour ledColourGreen;
extern const LedColour ledColourCyan;
extern const LedColour ledColourBlue;
extern const LedColour ledColourMagenta;
extern const LedColour ledColourWhite;
extern const LedColour ledColourBlack;

extern Led led0;
extern Led led1;
extern Led led2;
extern Led led3;
extern Led led4;
extern Led led5;
extern Led led6;
extern Led led7;
extern Led led8;
extern Led led9;
extern Led led10;
extern Led led11;
extern Led led12;
extern Led led13;
extern Led led14;
extern Led led15;
extern Led led16;
extern Led led17;
extern Led led18;
extern Led led19;
extern Led led20;

//------------------------------------------------------------------------------
// Function declaration

void LedInitialise(void);
void LedSet(Led * const led, const LedColour colour, const LedMode mode);
LedResult LedBlink(Led * const led, const LedColour colour);
LedResult LedStrobe(Led * const led);
void LedOverride(Led * const led, const LedColour colour);
void LedDisableOverride(Led * const led);

#endif

//------------------------------------------------------------------------------
// End of file
