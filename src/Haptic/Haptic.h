/**
 * @file Haptic.h
 * @author Seb Madgwick
 * @brief DRV2605L haptic driver.
 */

#ifndef HAPTIC_H
#define HAPTIC_H

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Result.
 */
typedef enum {
    HapticResultOK,
    HapticResultError,
} HapticResult;

//------------------------------------------------------------------------------
// Function prototypes

void HapticInitialise(void);
HapticResult HapticPlay(const int effect);

#endif

//------------------------------------------------------------------------------
// End of file
