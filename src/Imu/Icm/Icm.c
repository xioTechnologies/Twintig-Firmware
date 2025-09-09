/**
 * @file Icm.c
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"

//------------------------------------------------------------------------------
// Variables

const SpiSettings icmSpiSettings = {
    .clockFrequency = 14000000,
    .clockPolarity = SpiClockPolarityIdleHigh,
    .clockPhase = SpiClockPhaseIdleToActive,
};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Returns the test result message.
 * @param result Test result.
 * @return Test result message.
 */
const char* IcmTestResultToString(const IcmTestResult result) {
    switch (result) {
        case IcmTestResultPassed:
            return "Passed";
        case IcmTestResultInvalidId:
            return "Invalid ID";
        case IcmTestResultInterruptFailed:
            return "Interrupt failed";
    }
    return ""; // avoid compiler warning
}

//------------------------------------------------------------------------------
// End of file
