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
 * @brief Returns the AAF for an anti-aliasing.
 * @param antiAliasing Anti-aliasing.
 * @return AAF.
 */
IcmAaf IcmAntiAliasingToAaf(const IcmAntiAliasing antiAliasing) {
    switch (antiAliasing) {
        case IcmAntiAliasingDisabled:
            return icmAaf585Hz;
        case IcmAntiAliasing42Hz:
            return icmAaf42Hz;
        case IcmAntiAliasing84Hz:
            return icmAaf84Hz;
        case IcmAntiAliasing126Hz:
            return icmAaf126Hz;
        case IcmAntiAliasing170Hz:
            return icmAaf170Hz;
        case IcmAntiAliasing213Hz:
            return icmAaf213Hz;
        case IcmAntiAliasing258Hz:
            return icmAaf258Hz;
        case IcmAntiAliasing303Hz:
            return icmAaf303Hz;
        case IcmAntiAliasing348Hz:
            return icmAaf348Hz;
        case IcmAntiAliasing394Hz:
            return icmAaf394Hz;
        case IcmAntiAliasing441Hz:
            return icmAaf441Hz;
        case IcmAntiAliasing488Hz:
            return icmAaf488Hz;
        case IcmAntiAliasing536Hz:
            return icmAaf536Hz;
        case IcmAntiAliasing585Hz:
            return icmAaf585Hz;
        case IcmAntiAliasing634Hz:
            return icmAaf634Hz;
        case IcmAntiAliasing684Hz:
            return icmAaf684Hz;
        case IcmAntiAliasing734Hz:
            return icmAaf734Hz;
        case IcmAntiAliasing785Hz:
            return icmAaf785Hz;
        case IcmAntiAliasing837Hz:
            return icmAaf837Hz;
        case IcmAntiAliasing890Hz:
            return icmAaf890Hz;
        case IcmAntiAliasing943Hz:
            return icmAaf943Hz;
        case IcmAntiAliasing997Hz:
            return icmAaf997Hz;
        case IcmAntiAliasing1051Hz:
            return icmAaf1051Hz;
        case IcmAntiAliasing1107Hz:
            return icmAaf1107Hz;
        case IcmAntiAliasing1163Hz:
            return icmAaf1163Hz;
        case IcmAntiAliasing1220Hz:
            return icmAaf1220Hz;
        case IcmAntiAliasing1277Hz:
            return icmAaf1277Hz;
        case IcmAntiAliasing1336Hz:
            return icmAaf1336Hz;
        case IcmAntiAliasing1395Hz:
            return icmAaf1395Hz;
        case IcmAntiAliasing1454Hz:
            return icmAaf1454Hz;
        case IcmAntiAliasing1515Hz:
            return icmAaf1515Hz;
        case IcmAntiAliasing1577Hz:
            return icmAaf1577Hz;
        case IcmAntiAliasing1639Hz:
            return icmAaf1639Hz;
        case IcmAntiAliasing1702Hz:
            return icmAaf1702Hz;
        case IcmAntiAliasing1766Hz:
            return icmAaf1766Hz;
        case IcmAntiAliasing1830Hz:
            return icmAaf1830Hz;
        case IcmAntiAliasing1896Hz:
            return icmAaf1896Hz;
        case IcmAntiAliasing1962Hz:
            return icmAaf1962Hz;
        case IcmAntiAliasing2029Hz:
            return icmAaf2029Hz;
        case IcmAntiAliasing2097Hz:
            return icmAaf2097Hz;
        case IcmAntiAliasing2166Hz:
            return icmAaf2166Hz;
        case IcmAntiAliasing2235Hz:
            return icmAaf2235Hz;
        case IcmAntiAliasing2306Hz:
            return icmAaf2306Hz;
        case IcmAntiAliasing2377Hz:
            return icmAaf2377Hz;
        case IcmAntiAliasing2449Hz:
            return icmAaf2449Hz;
        case IcmAntiAliasing2522Hz:
            return icmAaf2522Hz;
        case IcmAntiAliasing2596Hz:
            return icmAaf2596Hz;
        case IcmAntiAliasing2671Hz:
            return icmAaf2671Hz;
        case IcmAntiAliasing2746Hz:
            return icmAaf2746Hz;
        case IcmAntiAliasing2823Hz:
            return icmAaf2823Hz;
        case IcmAntiAliasing2900Hz:
            return icmAaf2900Hz;
        case IcmAntiAliasing2978Hz:
            return icmAaf2978Hz;
        case IcmAntiAliasing3057Hz:
            return icmAaf3057Hz;
        case IcmAntiAliasing3137Hz:
            return icmAaf3137Hz;
        case IcmAntiAliasing3217Hz:
            return icmAaf3217Hz;
        case IcmAntiAliasing3299Hz:
            return icmAaf3299Hz;
        case IcmAntiAliasing3381Hz:
            return icmAaf3381Hz;
        case IcmAntiAliasing3464Hz:
            return icmAaf3464Hz;
        case IcmAntiAliasing3548Hz:
            return icmAaf3548Hz;
        case IcmAntiAliasing3633Hz:
            return icmAaf3633Hz;
        case IcmAntiAliasing3718Hz:
            return icmAaf3718Hz;
        case IcmAntiAliasing3805Hz:
            return icmAaf3805Hz;
        case IcmAntiAliasing3892Hz:
            return icmAaf3892Hz;
        case IcmAntiAliasing3979Hz:
            return icmAaf3979Hz;
    }
    return icmAaf585Hz; // avoid compiler warning
}

/**
 * @brief Returns the ODR value for a sample rate.
 * @param sampleRate Sample rate.
 * @return ODR value.
 */
int IcmSampleRateToOdr(const IcmSampleRate sampleRate) {
    switch (sampleRate) {
        case IcmSampleRate32kHz:
            return 0b0001;
        case IcmSampleRate16kHz:
            return 0b0010;
        case IcmSampleRate8kHz:
            return 0b0011;
        case IcmSampleRate4kHz:
            return 0b0100;
        case IcmSampleRate2kHz:
            return 0b0101;
        case IcmSampleRate1kHz:
            return 0b0110;
        case IcmSampleRate500Hz:
            return 0b1111;
        case IcmSampleRate200Hz:
            return 0b0111;
        case IcmSampleRate100Hz:
            return 0b1000;
        case IcmSampleRate50Hz:
            return 0b1001;
        case IcmSampleRate25Hz:
            return 0b1010;
        case IcmSampleRate12Hz:
            return 0b1011;
    }
    return 0b0110; // avoid compiler warning
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
        case IcmTestResultInvalidId:
            return "Invalid ID";
        case IcmTestResultInterruptFailed:
            return "Interrupt failed";
    }
    return ""; // avoid compiler warning
}

//------------------------------------------------------------------------------
// End of file
