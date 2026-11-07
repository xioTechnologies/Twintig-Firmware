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

static const IcmAaf aaf42Hz = {.delt = 1, .deltsqr = 1, .bitshift = 15};
static const IcmAaf aaf84Hz = {.delt = 2, .deltsqr = 4, .bitshift = 13};
static const IcmAaf aaf126Hz = {.delt = 3, .deltsqr = 9, .bitshift = 12};
static const IcmAaf aaf170Hz = {.delt = 4, .deltsqr = 16, .bitshift = 11};
static const IcmAaf aaf213Hz = {.delt = 5, .deltsqr = 25, .bitshift = 10};
static const IcmAaf aaf258Hz = {.delt = 6, .deltsqr = 36, .bitshift = 10};
static const IcmAaf aaf303Hz = {.delt = 7, .deltsqr = 49, .bitshift = 9};
static const IcmAaf aaf348Hz = {.delt = 8, .deltsqr = 64, .bitshift = 9};
static const IcmAaf aaf394Hz = {.delt = 9, .deltsqr = 81, .bitshift = 9};
static const IcmAaf aaf441Hz = {.delt = 10, .deltsqr = 100, .bitshift = 8};
static const IcmAaf aaf488Hz = {.delt = 11, .deltsqr = 122, .bitshift = 8};
static const IcmAaf aaf536Hz = {.delt = 12, .deltsqr = 144, .bitshift = 8};
static const IcmAaf aaf585Hz = {.delt = 13, .deltsqr = 170, .bitshift = 8};
static const IcmAaf aaf634Hz = {.delt = 14, .deltsqr = 196, .bitshift = 7};
static const IcmAaf aaf684Hz = {.delt = 15, .deltsqr = 224, .bitshift = 7};
static const IcmAaf aaf734Hz = {.delt = 16, .deltsqr = 256, .bitshift = 7};
static const IcmAaf aaf785Hz = {.delt = 17, .deltsqr = 288, .bitshift = 7};
static const IcmAaf aaf837Hz = {.delt = 18, .deltsqr = 324, .bitshift = 7};
static const IcmAaf aaf890Hz = {.delt = 19, .deltsqr = 360, .bitshift = 6};
static const IcmAaf aaf943Hz = {.delt = 20, .deltsqr = 400, .bitshift = 6};
static const IcmAaf aaf997Hz = {.delt = 21, .deltsqr = 440, .bitshift = 6};
static const IcmAaf aaf1051Hz = {.delt = 22, .deltsqr = 488, .bitshift = 6};
static const IcmAaf aaf1107Hz = {.delt = 23, .deltsqr = 528, .bitshift = 6};
static const IcmAaf aaf1163Hz = {.delt = 24, .deltsqr = 576, .bitshift = 6};
static const IcmAaf aaf1220Hz = {.delt = 25, .deltsqr = 624, .bitshift = 6};
static const IcmAaf aaf1277Hz = {.delt = 26, .deltsqr = 680, .bitshift = 6};
static const IcmAaf aaf1336Hz = {.delt = 27, .deltsqr = 736, .bitshift = 5};
static const IcmAaf aaf1395Hz = {.delt = 28, .deltsqr = 784, .bitshift = 5};
static const IcmAaf aaf1454Hz = {.delt = 29, .deltsqr = 848, .bitshift = 5};
static const IcmAaf aaf1515Hz = {.delt = 30, .deltsqr = 896, .bitshift = 5};
static const IcmAaf aaf1577Hz = {.delt = 31, .deltsqr = 960, .bitshift = 5};
static const IcmAaf aaf1639Hz = {.delt = 32, .deltsqr = 1024, .bitshift = 5};
static const IcmAaf aaf1702Hz = {.delt = 33, .deltsqr = 1088, .bitshift = 5};
static const IcmAaf aaf1766Hz = {.delt = 34, .deltsqr = 1152, .bitshift = 5};
static const IcmAaf aaf1830Hz = {.delt = 35, .deltsqr = 1232, .bitshift = 5};
static const IcmAaf aaf1896Hz = {.delt = 36, .deltsqr = 1296, .bitshift = 5};
static const IcmAaf aaf1962Hz = {.delt = 37, .deltsqr = 1376, .bitshift = 4};
static const IcmAaf aaf2029Hz = {.delt = 38, .deltsqr = 1440, .bitshift = 4};
static const IcmAaf aaf2097Hz = {.delt = 39, .deltsqr = 1536, .bitshift = 4};
static const IcmAaf aaf2166Hz = {.delt = 40, .deltsqr = 1600, .bitshift = 4};
static const IcmAaf aaf2235Hz = {.delt = 41, .deltsqr = 1696, .bitshift = 4};
static const IcmAaf aaf2306Hz = {.delt = 42, .deltsqr = 1760, .bitshift = 4};
static const IcmAaf aaf2377Hz = {.delt = 43, .deltsqr = 1856, .bitshift = 4};
static const IcmAaf aaf2449Hz = {.delt = 44, .deltsqr = 1952, .bitshift = 4};
static const IcmAaf aaf2522Hz = {.delt = 45, .deltsqr = 2016, .bitshift = 4};
static const IcmAaf aaf2596Hz = {.delt = 46, .deltsqr = 2112, .bitshift = 4};
static const IcmAaf aaf2671Hz = {.delt = 47, .deltsqr = 2208, .bitshift = 4};
static const IcmAaf aaf2746Hz = {.delt = 48, .deltsqr = 2304, .bitshift = 4};
static const IcmAaf aaf2823Hz = {.delt = 49, .deltsqr = 2400, .bitshift = 4};
static const IcmAaf aaf2900Hz = {.delt = 50, .deltsqr = 2496, .bitshift = 4};
static const IcmAaf aaf2978Hz = {.delt = 51, .deltsqr = 2592, .bitshift = 4};
static const IcmAaf aaf3057Hz = {.delt = 52, .deltsqr = 2720, .bitshift = 4};
static const IcmAaf aaf3137Hz = {.delt = 53, .deltsqr = 2816, .bitshift = 3};
static const IcmAaf aaf3217Hz = {.delt = 54, .deltsqr = 2944, .bitshift = 3};
static const IcmAaf aaf3299Hz = {.delt = 55, .deltsqr = 3008, .bitshift = 3};
static const IcmAaf aaf3381Hz = {.delt = 56, .deltsqr = 3136, .bitshift = 3};
static const IcmAaf aaf3464Hz = {.delt = 57, .deltsqr = 3264, .bitshift = 3};
static const IcmAaf aaf3548Hz = {.delt = 58, .deltsqr = 3392, .bitshift = 3};
static const IcmAaf aaf3633Hz = {.delt = 59, .deltsqr = 3456, .bitshift = 3};
static const IcmAaf aaf3718Hz = {.delt = 60, .deltsqr = 3584, .bitshift = 3};
static const IcmAaf aaf3805Hz = {.delt = 61, .deltsqr = 3712, .bitshift = 3};
static const IcmAaf aaf3892Hz = {.delt = 62, .deltsqr = 3840, .bitshift = 3};
static const IcmAaf aaf3979Hz = {.delt = 63, .deltsqr = 3968, .bitshift = 3};

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
            return aaf585Hz;
        case IcmAntiAliasing42Hz:
            return aaf42Hz;
        case IcmAntiAliasing84Hz:
            return aaf84Hz;
        case IcmAntiAliasing126Hz:
            return aaf126Hz;
        case IcmAntiAliasing170Hz:
            return aaf170Hz;
        case IcmAntiAliasing213Hz:
            return aaf213Hz;
        case IcmAntiAliasing258Hz:
            return aaf258Hz;
        case IcmAntiAliasing303Hz:
            return aaf303Hz;
        case IcmAntiAliasing348Hz:
            return aaf348Hz;
        case IcmAntiAliasing394Hz:
            return aaf394Hz;
        case IcmAntiAliasing441Hz:
            return aaf441Hz;
        case IcmAntiAliasing488Hz:
            return aaf488Hz;
        case IcmAntiAliasing536Hz:
            return aaf536Hz;
        case IcmAntiAliasing585Hz:
            return aaf585Hz;
        case IcmAntiAliasing634Hz:
            return aaf634Hz;
        case IcmAntiAliasing684Hz:
            return aaf684Hz;
        case IcmAntiAliasing734Hz:
            return aaf734Hz;
        case IcmAntiAliasing785Hz:
            return aaf785Hz;
        case IcmAntiAliasing837Hz:
            return aaf837Hz;
        case IcmAntiAliasing890Hz:
            return aaf890Hz;
        case IcmAntiAliasing943Hz:
            return aaf943Hz;
        case IcmAntiAliasing997Hz:
            return aaf997Hz;
        case IcmAntiAliasing1051Hz:
            return aaf1051Hz;
        case IcmAntiAliasing1107Hz:
            return aaf1107Hz;
        case IcmAntiAliasing1163Hz:
            return aaf1163Hz;
        case IcmAntiAliasing1220Hz:
            return aaf1220Hz;
        case IcmAntiAliasing1277Hz:
            return aaf1277Hz;
        case IcmAntiAliasing1336Hz:
            return aaf1336Hz;
        case IcmAntiAliasing1395Hz:
            return aaf1395Hz;
        case IcmAntiAliasing1454Hz:
            return aaf1454Hz;
        case IcmAntiAliasing1515Hz:
            return aaf1515Hz;
        case IcmAntiAliasing1577Hz:
            return aaf1577Hz;
        case IcmAntiAliasing1639Hz:
            return aaf1639Hz;
        case IcmAntiAliasing1702Hz:
            return aaf1702Hz;
        case IcmAntiAliasing1766Hz:
            return aaf1766Hz;
        case IcmAntiAliasing1830Hz:
            return aaf1830Hz;
        case IcmAntiAliasing1896Hz:
            return aaf1896Hz;
        case IcmAntiAliasing1962Hz:
            return aaf1962Hz;
        case IcmAntiAliasing2029Hz:
            return aaf2029Hz;
        case IcmAntiAliasing2097Hz:
            return aaf2097Hz;
        case IcmAntiAliasing2166Hz:
            return aaf2166Hz;
        case IcmAntiAliasing2235Hz:
            return aaf2235Hz;
        case IcmAntiAliasing2306Hz:
            return aaf2306Hz;
        case IcmAntiAliasing2377Hz:
            return aaf2377Hz;
        case IcmAntiAliasing2449Hz:
            return aaf2449Hz;
        case IcmAntiAliasing2522Hz:
            return aaf2522Hz;
        case IcmAntiAliasing2596Hz:
            return aaf2596Hz;
        case IcmAntiAliasing2671Hz:
            return aaf2671Hz;
        case IcmAntiAliasing2746Hz:
            return aaf2746Hz;
        case IcmAntiAliasing2823Hz:
            return aaf2823Hz;
        case IcmAntiAliasing2900Hz:
            return aaf2900Hz;
        case IcmAntiAliasing2978Hz:
            return aaf2978Hz;
        case IcmAntiAliasing3057Hz:
            return aaf3057Hz;
        case IcmAntiAliasing3137Hz:
            return aaf3137Hz;
        case IcmAntiAliasing3217Hz:
            return aaf3217Hz;
        case IcmAntiAliasing3299Hz:
            return aaf3299Hz;
        case IcmAntiAliasing3381Hz:
            return aaf3381Hz;
        case IcmAntiAliasing3464Hz:
            return aaf3464Hz;
        case IcmAntiAliasing3548Hz:
            return aaf3548Hz;
        case IcmAntiAliasing3633Hz:
            return aaf3633Hz;
        case IcmAntiAliasing3718Hz:
            return aaf3718Hz;
        case IcmAntiAliasing3805Hz:
            return aaf3805Hz;
        case IcmAntiAliasing3892Hz:
            return aaf3892Hz;
        case IcmAntiAliasing3979Hz:
            return aaf3979Hz;
    }
    return aaf585Hz; // unreachable code: -Werror=switch
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
    return 0; // unreachable code: -Werror=switch
}

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
    return ""; // unreachable code: -Werror=switch
}

//------------------------------------------------------------------------------
// End of file
