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
                .timestamp = icmData.timestamp,
                .gyroscope = gyroscope,
                .accelerometer = accelerometer,
            };
            imu->inertialDataReady(&inertialData, imu->context);
        }

        // Temperature callback
        if (imu->temperatureDataReady != NULL) {
            ImuTemperatureData temperatureData = {
                .timestamp = icmData.timestamp,
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
        const bool invalid = imu->previousTimestamp == 0;
        const float deltaTime = (float) ((double) (icmData.timestamp - imu->previousTimestamp) * (1.0 / (double) TIMER_TICKS_PER_SECOND));
        imu->previousTimestamp = icmData.timestamp;
        if (invalid) {
            continue;
        }

        // Update AHRS
        FusionAhrsUpdateNoMagnetometer(&imu->ahrs, gyroscope, accelerometer, deltaTime);

        // AHRS callback
        if (imu->ahrsDataReady != NULL) {
            ImuAhrsData ahrsData = {
                .timestamp = icmData.timestamp,
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
    if ((imu->initialised == false) || (imu->settings.sampleRate != settings->sampleRate)) {
        imu->icm->initialise(SampleRateToOdr(settings->sampleRate));
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
        imu->previousTimestamp = 0;
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
