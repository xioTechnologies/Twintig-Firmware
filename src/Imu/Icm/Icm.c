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

const IcmAaf icmAaf42Hz = {.delt = 1, .deltsqr = 1, .bitshift = 15};
const IcmAaf icmAaf84Hz = {.delt = 2, .deltsqr = 4, .bitshift = 13};
const IcmAaf icmAaf126Hz = {.delt = 3, .deltsqr = 9, .bitshift = 12};
const IcmAaf icmAaf170Hz = {.delt = 4, .deltsqr = 16, .bitshift = 11};
const IcmAaf icmAaf213Hz = {.delt = 5, .deltsqr = 25, .bitshift = 10};
const IcmAaf icmAaf258Hz = {.delt = 6, .deltsqr = 36, .bitshift = 10};
const IcmAaf icmAaf303Hz = {.delt = 7, .deltsqr = 49, .bitshift = 9};
const IcmAaf icmAaf348Hz = {.delt = 8, .deltsqr = 64, .bitshift = 9};
const IcmAaf icmAaf394Hz = {.delt = 9, .deltsqr = 81, .bitshift = 9};
const IcmAaf icmAaf441Hz = {.delt = 10, .deltsqr = 100, .bitshift = 8};
const IcmAaf icmAaf488Hz = {.delt = 11, .deltsqr = 122, .bitshift = 8};
const IcmAaf icmAaf536Hz = {.delt = 12, .deltsqr = 144, .bitshift = 8};
const IcmAaf icmAaf585Hz = {.delt = 13, .deltsqr = 170, .bitshift = 8};
const IcmAaf icmAaf634Hz = {.delt = 14, .deltsqr = 196, .bitshift = 7};
const IcmAaf icmAaf684Hz = {.delt = 15, .deltsqr = 224, .bitshift = 7};
const IcmAaf icmAaf734Hz = {.delt = 16, .deltsqr = 256, .bitshift = 7};
const IcmAaf icmAaf785Hz = {.delt = 17, .deltsqr = 288, .bitshift = 7};
const IcmAaf icmAaf837Hz = {.delt = 18, .deltsqr = 324, .bitshift = 7};
const IcmAaf icmAaf890Hz = {.delt = 19, .deltsqr = 360, .bitshift = 6};
const IcmAaf icmAaf943Hz = {.delt = 20, .deltsqr = 400, .bitshift = 6};
const IcmAaf icmAaf997Hz = {.delt = 21, .deltsqr = 440, .bitshift = 6};
const IcmAaf icmAaf1051Hz = {.delt = 22, .deltsqr = 488, .bitshift = 6};
const IcmAaf icmAaf1107Hz = {.delt = 23, .deltsqr = 528, .bitshift = 6};
const IcmAaf icmAaf1163Hz = {.delt = 24, .deltsqr = 576, .bitshift = 6};
const IcmAaf icmAaf1220Hz = {.delt = 25, .deltsqr = 624, .bitshift = 6};
const IcmAaf icmAaf1277Hz = {.delt = 26, .deltsqr = 680, .bitshift = 6};
const IcmAaf icmAaf1336Hz = {.delt = 27, .deltsqr = 736, .bitshift = 5};
const IcmAaf icmAaf1395Hz = {.delt = 28, .deltsqr = 784, .bitshift = 5};
const IcmAaf icmAaf1454Hz = {.delt = 29, .deltsqr = 848, .bitshift = 5};
const IcmAaf icmAaf1515Hz = {.delt = 30, .deltsqr = 896, .bitshift = 5};
const IcmAaf icmAaf1577Hz = {.delt = 31, .deltsqr = 960, .bitshift = 5};
const IcmAaf icmAaf1639Hz = {.delt = 32, .deltsqr = 1024, .bitshift = 5};
const IcmAaf icmAaf1702Hz = {.delt = 33, .deltsqr = 1088, .bitshift = 5};
const IcmAaf icmAaf1766Hz = {.delt = 34, .deltsqr = 1152, .bitshift = 5};
const IcmAaf icmAaf1830Hz = {.delt = 35, .deltsqr = 1232, .bitshift = 5};
const IcmAaf icmAaf1896Hz = {.delt = 36, .deltsqr = 1296, .bitshift = 5};
const IcmAaf icmAaf1962Hz = {.delt = 37, .deltsqr = 1376, .bitshift = 4};
const IcmAaf icmAaf2029Hz = {.delt = 38, .deltsqr = 1440, .bitshift = 4};
const IcmAaf icmAaf2097Hz = {.delt = 39, .deltsqr = 1536, .bitshift = 4};
const IcmAaf icmAaf2166Hz = {.delt = 40, .deltsqr = 1600, .bitshift = 4};
const IcmAaf icmAaf2235Hz = {.delt = 41, .deltsqr = 1696, .bitshift = 4};
const IcmAaf icmAaf2306Hz = {.delt = 42, .deltsqr = 1760, .bitshift = 4};
const IcmAaf icmAaf2377Hz = {.delt = 43, .deltsqr = 1856, .bitshift = 4};
const IcmAaf icmAaf2449Hz = {.delt = 44, .deltsqr = 1952, .bitshift = 4};
const IcmAaf icmAaf2522Hz = {.delt = 45, .deltsqr = 2016, .bitshift = 4};
const IcmAaf icmAaf2596Hz = {.delt = 46, .deltsqr = 2112, .bitshift = 4};
const IcmAaf icmAaf2671Hz = {.delt = 47, .deltsqr = 2208, .bitshift = 4};
const IcmAaf icmAaf2746Hz = {.delt = 48, .deltsqr = 2304, .bitshift = 4};
const IcmAaf icmAaf2823Hz = {.delt = 49, .deltsqr = 2400, .bitshift = 4};
const IcmAaf icmAaf2900Hz = {.delt = 50, .deltsqr = 2496, .bitshift = 4};
const IcmAaf icmAaf2978Hz = {.delt = 51, .deltsqr = 2592, .bitshift = 4};
const IcmAaf icmAaf3057Hz = {.delt = 52, .deltsqr = 2720, .bitshift = 4};
const IcmAaf icmAaf3137Hz = {.delt = 53, .deltsqr = 2816, .bitshift = 3};
const IcmAaf icmAaf3217Hz = {.delt = 54, .deltsqr = 2944, .bitshift = 3};
const IcmAaf icmAaf3299Hz = {.delt = 55, .deltsqr = 3008, .bitshift = 3};
const IcmAaf icmAaf3381Hz = {.delt = 56, .deltsqr = 3136, .bitshift = 3};
const IcmAaf icmAaf3464Hz = {.delt = 57, .deltsqr = 3264, .bitshift = 3};
const IcmAaf icmAaf3548Hz = {.delt = 58, .deltsqr = 3392, .bitshift = 3};
const IcmAaf icmAaf3633Hz = {.delt = 59, .deltsqr = 3456, .bitshift = 3};
const IcmAaf icmAaf3718Hz = {.delt = 60, .deltsqr = 3584, .bitshift = 3};
const IcmAaf icmAaf3805Hz = {.delt = 61, .deltsqr = 3712, .bitshift = 3};
const IcmAaf icmAaf3892Hz = {.delt = 62, .deltsqr = 3840, .bitshift = 3};
const IcmAaf icmAaf3979Hz = {.delt = 63, .deltsqr = 3968, .bitshift = 3};

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
