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
 * @brief Blink queue item.
 */
typedef struct {
    LedColour colour;
    bool pending;
} LedBlinkQueueItem;

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
    LedBlinkQueueItem blinkQueue[LED_BLINK_QUEUE_LENGTH];
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

extern Led ledMain;
extern Led ledA;
extern Led ledB;
extern Led ledC;
extern Led ledD;
extern Led ledE;
extern Led ledF;
extern Led ledG;
extern Led ledH;
extern Led ledI;
extern Led ledJ;
extern Led ledK;
extern Led ledL;
extern Led ledM;
extern Led ledN;
extern Led ledO;
extern Led ledP;
extern Led ledQ;
extern Led ledR;
extern Led ledS;
extern Led ledT;

//------------------------------------------------------------------------------
// Function declarations

void LedInitialise(void);
void LedSet(Led * const led, const LedColour colour, const LedMode mode);
LedResult LedBlink(Led * const led, const LedColour colour);
LedResult LedStrobe(Led * const led);
void LedOverride(Led * const led, const LedColour colour);
void LedDisableOverride(Led * const led);

#endif

//------------------------------------------------------------------------------
// End of file
