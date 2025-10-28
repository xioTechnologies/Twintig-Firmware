/**
 * @file Imu.c
 * @author Seb Madgwick
 * @brief IMU processing.
 */

//------------------------------------------------------------------------------
// Includes

#include "Imu.h"
#include "Imu/Icm/Icm1.h"
#include "Imu/Icm/Icm10.h"
#include "Imu/Icm/Icm11.h"
#include "Imu/Icm/Icm12.h"
#include "Imu/Icm/Icm13.h"
#include "Imu/Icm/Icm14.h"
#include "Imu/Icm/Icm15.h"
#include "Imu/Icm/Icm16.h"
#include "Imu/Icm/Icm17.h"
#include "Imu/Icm/Icm18.h"
#include "Imu/Icm/Icm19.h"
#include "Imu/Icm/Icm2.h"
#include "Imu/Icm/Icm20.h"
#include "Imu/Icm/Icm3.h"
#include "Imu/Icm/Icm4.h"
#include "Imu/Icm/Icm5.h"
#include "Imu/Icm/Icm6.h"
#include "Imu/Icm/Icm7.h"
#include "Imu/Icm/Icm8.h"
#include "Imu/Icm/Icm9.h"
#include <stddef.h>
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Function declarations

static IcmAaf AntiAliasingToAaf(const ImuAntiAliasing antiAliasing);
static IcmOdr SampleRateToOdr(const ImuSampleRate sampleRate);

//------------------------------------------------------------------------------
// Variables

Imu imu1 = {.icm = &icm1};
Imu imu2 = {.icm = &icm2};
Imu imu3 = {.icm = &icm3};
Imu imu4 = {.icm = &icm4};
Imu imu5 = {.icm = &icm5};
Imu imu6 = {.icm = &icm6};
Imu imu7 = {.icm = &icm7};
Imu imu8 = {.icm = &icm8};
Imu imu9 = {.icm = &icm9};
Imu imu10 = {.icm = &icm10};
Imu imu11 = {.icm = &icm11};
Imu imu12 = {.icm = &icm12};
Imu imu13 = {.icm = &icm13};
Imu imu14 = {.icm = &icm14};
Imu imu15 = {.icm = &icm15};
Imu imu16 = {.icm = &icm16};
Imu imu17 = {.icm = &icm17};
Imu imu18 = {.icm = &icm18};
Imu imu19 = {.icm = &icm19};
Imu imu20 = {.icm = &icm20};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 * @param imu IMU structure.
 */
void ImuTasks(Imu * const imu) {

    // Do nothing if not initialised
    if (imu->initialised == false) {
        return;
    }

    // Process available data
    int numberOfSamples = 0;
    while (numberOfSamples++ < 8) {

        // Get data
        IcmData icmData;
        if (imu->icm->getData(&icmData) != IcmResultOk) {
            break;
        }

        // Apply calibration
        FusionVector gyroscope = {
            .axis.x = icmData.gyroscopeX,
            .axis.y = icmData.gyroscopeY,
            .axis.z = icmData.gyroscopeZ,
        };
        FusionVector accelerometer = {
            .axis.x = icmData.accelerometerX,
            .axis.y = icmData.accelerometerY,
            .axis.z = icmData.accelerometerZ,
        };
        gyroscope = FusionCalibrationInertial(gyroscope, imu->settings.gyroscopeMisalignment, imu->settings.gyroscopeSensitivity, imu->settings.gyroscopeOffset);
        accelerometer = FusionCalibrationInertial(accelerometer, imu->settings.accelerometerMisalignment, imu->settings.accelerometerSensitivity, imu->settings.accelerometerOffset);

        // Update offset
        if (imu->settings.gyroscopeOffsetEnabled) {
            gyroscope = FusionOffsetUpdate(&imu->offset, gyroscope);
        }

        // Axis alignment
        gyroscope = FusionAxesSwap(gyroscope, imu->settings.axisAlignment);
        accelerometer = FusionAxesSwap(accelerometer, imu->settings.axisAlignment);

        // Inertial callback
        if (imu->inertialDataReady != NULL) {
            ImuInertialData inertialData = {
                .ticks = icmData.ticks,
                .gyroscope = gyroscope,
                .accelerometer = accelerometer,
            };
            imu->inertialDataReady(&inertialData, imu->context);
        }

        // Temperature callback
        if (imu->temperatureDataReady != NULL) {
            ImuTemperatureData temperatureData = {
                .ticks = icmData.ticks,
                .temperature = icmData.temperature,
            };
            imu->temperatureDataReady(&temperatureData, imu->context);
        }

        // Downsampling
        if (imu->settings.ahrsUpdateRateDivisor == 0) {
            continue;
        }
        if (imu->settings.ahrsUpdateRateDivisor > 1) {
            if (imu->downsampledCount == 0) {
                imu->downsampledGyroscope = FUSION_VECTOR_ZERO;
                imu->downsampledAccelerometer = FUSION_VECTOR_ZERO;
            }
            imu->downsampledGyroscope = FusionVectorAdd(imu->downsampledGyroscope, gyroscope);
            imu->downsampledAccelerometer = FusionVectorAdd(imu->downsampledAccelerometer, accelerometer);
            if (++imu->downsampledCount < imu->settings.ahrsUpdateRateDivisor) {
                continue;
            }
            const float reciprocal = 1.0f / (float) imu->downsampledCount;
            gyroscope = FusionVectorMultiplyScalar(imu->downsampledGyroscope, reciprocal);
            accelerometer = FusionVectorMultiplyScalar(imu->downsampledAccelerometer, reciprocal);
        }
        imu->downsampledCount = 0;

        // Calculate delta time
        const bool invalid = imu->previousTicks == 0;
        const float deltaTime = (float) ((double) (icmData.ticks - imu->previousTicks) * (1.0 / (double) TIMER_TICKS_PER_SECOND));
        imu->previousTicks = icmData.ticks;
        if (invalid) {
            continue;
        }

        // Update AHRS
        FusionAhrsUpdateNoMagnetometer(&imu->ahrs, gyroscope, accelerometer, deltaTime);

        // AHRS callback
        if (imu->ahrsDataReady != NULL) {
            ImuAhrsData ahrsData = {
                .ticks = icmData.ticks,
                .ahrs = &imu->ahrs,
            };
            imu->ahrsDataReady(&ahrsData, imu->context);
        }
    }
}

/**
 * @brief Sets the settings.
 * @param imu IMU structure.
 * @param settings Settings.
 */
void ImuSetSettings(Imu * const imu, const ImuSettings * const settings) {

    // Initialise hardware
    const bool uninitialised = imu->initialised == false;
    const bool gyroscopeNotchFilterEnabledChanged = imu->settings.gyroscopeNotchFilterEnabled != settings->gyroscopeNotchFilterEnabled;
    const bool gyroscopeAntiAliasingChanged = imu->settings.gyroscopeAntiAliasing != settings->gyroscopeAntiAliasing;
    const bool accelerometerAntiAliasingChanged = imu->settings.accelerometerAntiAliasing != settings->accelerometerAntiAliasing;
    const bool sampleRateChanged = imu->settings.sampleRate != settings->sampleRate;
    if (uninitialised || gyroscopeNotchFilterEnabledChanged || gyroscopeAntiAliasingChanged || accelerometerAntiAliasingChanged || sampleRateChanged) {
        const IcmSettings icmSettings = {
            .gyroscopeNfDisable = settings->gyroscopeNotchFilterEnabled ? false : true,
            .gyroscopeAaf = AntiAliasingToAaf(settings->gyroscopeAntiAliasing),
            .gyroscopeAafDisable = settings->gyroscopeAntiAliasing == ImuAntiAliasingDisabled,
            .accelerometerAaf = AntiAliasingToAaf(settings->accelerometerAntiAliasing),
            .accelerometerAafDisable = settings->accelerometerAntiAliasing == ImuAntiAliasingDisabled,
            .odr = SampleRateToOdr(settings->sampleRate),
        };
        imu->icm->initialise(&icmSettings);
        FusionOffsetInitialise(&imu->offset, (unsigned int) settings->sampleRate);
    }

    // Initialise AHRS
    if (imu->initialised == false) {
        FusionAhrsInitialise(&imu->ahrs);
    }

    // Reset AHRS
    if ((imu->settings.axisAlignment != settings->axisAlignment) ||
        (imu->settings.ahrsAxesConvention != settings->ahrsAxesConvention) ||
        (imu->settings.ahrsUpdateRateDivisor != settings->ahrsUpdateRateDivisor)) {
        FusionAhrsReset(&imu->ahrs);
        imu->previousTicks = 0;
    }

    // Update settings
    imu->settings = *settings;

    // Set AHRS settings
    const FusionAhrsSettings ahrsSettings = (FusionAhrsSettings){
        .convention = imu->settings.ahrsAxesConvention,
        .gain = imu->settings.ahrsGain,
        .gyroscopeRange = 2000.0f,
        .accelerationRejection = imu->settings.ahrsAccelerationRejection,
        .recoveryTriggerPeriod = 10 * 100,
    };
    FusionAhrsSetSettings(&imu->ahrs, &ahrsSettings);

    // Set flag
    imu->initialised = true;
}

/**
 * @brief Returns the AAF for an anti-aliasing.
 * @param antiAliasing Anti-aliasing.
 * @return AAF.
 */
static IcmAaf AntiAliasingToAaf(const ImuAntiAliasing antiAliasing) {
    switch (antiAliasing) {
        case ImuAntiAliasingDisabled:
            return icmAaf585Hz;
        case ImuAntiAliasing42Hz:
            return icmAaf42Hz;
        case ImuAntiAliasing84Hz:
            return icmAaf84Hz;
        case ImuAntiAliasing126Hz:
            return icmAaf126Hz;
        case ImuAntiAliasing170Hz:
            return icmAaf170Hz;
        case ImuAntiAliasing213Hz:
            return icmAaf213Hz;
        case ImuAntiAliasing258Hz:
            return icmAaf258Hz;
        case ImuAntiAliasing303Hz:
            return icmAaf303Hz;
        case ImuAntiAliasing348Hz:
            return icmAaf348Hz;
        case ImuAntiAliasing394Hz:
            return icmAaf394Hz;
        case ImuAntiAliasing441Hz:
            return icmAaf441Hz;
        case ImuAntiAliasing488Hz:
            return icmAaf488Hz;
        case ImuAntiAliasing536Hz:
            return icmAaf536Hz;
        case ImuAntiAliasing585Hz:
            return icmAaf585Hz;
        case ImuAntiAliasing634Hz:
            return icmAaf634Hz;
        case ImuAntiAliasing684Hz:
            return icmAaf684Hz;
        case ImuAntiAliasing734Hz:
            return icmAaf734Hz;
        case ImuAntiAliasing785Hz:
            return icmAaf785Hz;
        case ImuAntiAliasing837Hz:
            return icmAaf837Hz;
        case ImuAntiAliasing890Hz:
            return icmAaf890Hz;
        case ImuAntiAliasing943Hz:
            return icmAaf943Hz;
        case ImuAntiAliasing997Hz:
            return icmAaf997Hz;
        case ImuAntiAliasing1051Hz:
            return icmAaf1051Hz;
        case ImuAntiAliasing1107Hz:
            return icmAaf1107Hz;
        case ImuAntiAliasing1163Hz:
            return icmAaf1163Hz;
        case ImuAntiAliasing1220Hz:
            return icmAaf1220Hz;
        case ImuAntiAliasing1277Hz:
            return icmAaf1277Hz;
        case ImuAntiAliasing1336Hz:
            return icmAaf1336Hz;
        case ImuAntiAliasing1395Hz:
            return icmAaf1395Hz;
        case ImuAntiAliasing1454Hz:
            return icmAaf1454Hz;
        case ImuAntiAliasing1515Hz:
            return icmAaf1515Hz;
        case ImuAntiAliasing1577Hz:
            return icmAaf1577Hz;
        case ImuAntiAliasing1639Hz:
            return icmAaf1639Hz;
        case ImuAntiAliasing1702Hz:
            return icmAaf1702Hz;
        case ImuAntiAliasing1766Hz:
            return icmAaf1766Hz;
        case ImuAntiAliasing1830Hz:
            return icmAaf1830Hz;
        case ImuAntiAliasing1896Hz:
            return icmAaf1896Hz;
        case ImuAntiAliasing1962Hz:
            return icmAaf1962Hz;
        case ImuAntiAliasing2029Hz:
            return icmAaf2029Hz;
        case ImuAntiAliasing2097Hz:
            return icmAaf2097Hz;
        case ImuAntiAliasing2166Hz:
            return icmAaf2166Hz;
        case ImuAntiAliasing2235Hz:
            return icmAaf2235Hz;
        case ImuAntiAliasing2306Hz:
            return icmAaf2306Hz;
        case ImuAntiAliasing2377Hz:
            return icmAaf2377Hz;
        case ImuAntiAliasing2449Hz:
            return icmAaf2449Hz;
        case ImuAntiAliasing2522Hz:
            return icmAaf2522Hz;
        case ImuAntiAliasing2596Hz:
            return icmAaf2596Hz;
        case ImuAntiAliasing2671Hz:
            return icmAaf2671Hz;
        case ImuAntiAliasing2746Hz:
            return icmAaf2746Hz;
        case ImuAntiAliasing2823Hz:
            return icmAaf2823Hz;
        case ImuAntiAliasing2900Hz:
            return icmAaf2900Hz;
        case ImuAntiAliasing2978Hz:
            return icmAaf2978Hz;
        case ImuAntiAliasing3057Hz:
            return icmAaf3057Hz;
        case ImuAntiAliasing3137Hz:
            return icmAaf3137Hz;
        case ImuAntiAliasing3217Hz:
            return icmAaf3217Hz;
        case ImuAntiAliasing3299Hz:
            return icmAaf3299Hz;
        case ImuAntiAliasing3381Hz:
            return icmAaf3381Hz;
        case ImuAntiAliasing3464Hz:
            return icmAaf3464Hz;
        case ImuAntiAliasing3548Hz:
            return icmAaf3548Hz;
        case ImuAntiAliasing3633Hz:
            return icmAaf3633Hz;
        case ImuAntiAliasing3718Hz:
            return icmAaf3718Hz;
        case ImuAntiAliasing3805Hz:
            return icmAaf3805Hz;
        case ImuAntiAliasing3892Hz:
            return icmAaf3892Hz;
        case ImuAntiAliasing3979Hz:
            return icmAaf3979Hz;
    }
    return icmAaf585Hz; // avoid compiler warning
}

/**
 * @brief Returns the ODR for a sample rate.
 * @param sampleRate Sample rate.
 * @return ODR.
 */
static IcmOdr SampleRateToOdr(const ImuSampleRate sampleRate) {
    switch (sampleRate) {
        case ImuSampleRate32kHz:
            return IcmOdr32kHz;
        case ImuSampleRate16kHz:
            return IcmOdr16kHz;
        case ImuSampleRate8kHz:
            return IcmOdr8kHz;
        case ImuSampleRate4kHz:
            return IcmOdr4kHz;
        case ImuSampleRate2kHz:
            return IcmOdr2kHz;
        case ImuSampleRate1kHz:
            return IcmOdr1kHz;
        case ImuSampleRate500Hz:
            return IcmOdr500Hz;
        case ImuSampleRate200Hz:
            return IcmOdr200Hz;
        case ImuSampleRate100Hz:
            return IcmOdr100Hz;
        case ImuSampleRate50Hz:
            return IcmOdr50Hz;
        case ImuSampleRate25Hz:
            return IcmOdr25Hz;
        case ImuSampleRate12Hz:
            return IcmOdr12Hz;
    }
    return IcmOdr1kHz; // avoid compiler warning
}

/**
 * @brief Resets the AHRS algorithm.
 * @param imu IMU structure.
 */
void ImuReset(Imu * const imu) {
    FusionAhrsReset(&imu->ahrs);
}

/**
 * @brief Sets the heading of the orientation measurement provided by the AHRS
 * algorithm.
 * @param imu IMU structure.
 * @param heading Heading angle in degrees.
 */
void ImuSetHeading(Imu * const imu, const float heading) {
    FusionAhrsSetHeading(&imu->ahrs, heading);
}

//------------------------------------------------------------------------------
// End of file
