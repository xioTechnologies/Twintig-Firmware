/**
 * @file Icm.c
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
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
 * @brief Returns a string representation of the result.
 * @param result Result.
 * @return String representation of the result.
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
