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
    HapticResultOk,
    HapticResultError,
} HapticResult;

/**
 * @brief Test result.
 */
typedef enum {
    HapticTestResultPassed,
    HapticTestResultAckFailed,
    HapticTestResultInvalidId,
    HapticTestResultDiagnosticsFailed,
    HapticTestResultOverTemperature,
    HapticTestResultOverCurrent,
} HapticTestResult;

//------------------------------------------------------------------------------
// Function prototypes

void HapticInitialise(void);
HapticResult HapticPlay(const int effect);
HapticTestResult HapticTest(void);
const char* HapticTestResultToString(const HapticTestResult result);

#endif

//------------------------------------------------------------------------------
// End of file
