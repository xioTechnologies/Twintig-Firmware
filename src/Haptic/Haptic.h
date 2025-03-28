/**
 * @file Haptic.h
 * @author Seb Madgwick
 * @brief Driver for the DRV2605L haptic driver.
 */

#ifndef HAPTIC_H
#define HAPTIC_H

//------------------------------------------------------------------------------
// Function prototypes

void HapticInitialise(void);
int HapticPlay(const int effect);

#endif

//------------------------------------------------------------------------------
// End of file
