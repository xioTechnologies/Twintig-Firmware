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
    .clockFrequency = 16000000,
    .clockPolarity = SpiClockPolarityIdleHigh,
    .clockPhase = SpiClockPhaseIdleToActive,
};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Returns ODR as float.
 * @param odr ODR.
 * @return ODR as float.
 */
float IcmOdrToFloat(const IcmOdr odr) {
    switch (odr) {
        case IcmOdr32kHz:
            return 32000.0f;
        case IcmOdr16kHz:
            return 16000.0f;
        case IcmOdr8kHz:
            return 8000.0f;
        case IcmOdr4kHz:
            return 4000.0f;
        case IcmOdr2kHz:
            return 2000.0f;
        case IcmOdr1kHz:
            return 1000.0f;
        case IcmOdr200Hz:
            return 200.0f;
        case IcmOdr100Hz:
            return 100.0f;
        case IcmOdr50Hz:
            return 50.0f;
        case IcmOdr25Hz:
            return 25.0f;
        case IcmOdr12Hz:
            return 12.5f;
        case IcmOdr500Hz:
            return 500.0f;
    }
    return 0.0f; // avoid compiler warning
}

/**
 * @brief Returns the test result message.
 * @param result Test result.
 * @return Test result message.
 */
const char* IcmTestResultToString(const IcmTestResult result) {
    switch (result) {
        case IcmTestResultPassed:
            return "Passed";
        case IcmTestResultInvalidID:
            return "Invalid ID";
        case IcmTestResultInterruptFailed:
            return "Interrupt failed";
    }
    return ""; // avoid compiler warning
}

//------------------------------------------------------------------------------
// End of file
