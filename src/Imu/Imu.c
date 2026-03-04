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
// Variables

Imu imu1 = {.icm = &icm1, .send = &send1};
Imu imu2 = {.icm = &icm2, .send = &send2};
Imu imu3 = {.icm = &icm3, .send = &send3};
Imu imu4 = {.icm = &icm4, .send = &send4};
Imu imu5 = {.icm = &icm5, .send = &send5};
Imu imu6 = {.icm = &icm6, .send = &send6};
Imu imu7 = {.icm = &icm7, .send = &send7};
Imu imu8 = {.icm = &icm8, .send = &send8};
Imu imu9 = {.icm = &icm9, .send = &send9};
Imu imu10 = {.icm = &icm10, .send = &send10};
Imu imu11 = {.icm = &icm11, .send = &send11};
Imu imu12 = {.icm = &icm12, .send = &send12};
Imu imu13 = {.icm = &icm13, .send = &send13};
Imu imu14 = {.icm = &icm14, .send = &send14};
Imu imu15 = {.icm = &icm15, .send = &send15};
Imu imu16 = {.icm = &icm16, .send = &send16};
Imu imu17 = {.icm = &icm17, .send = &send17};
Imu imu18 = {.icm = &icm18, .send = &send18};
Imu imu19 = {.icm = &icm19, .send = &send19};
Imu imu20 = {.icm = &icm20, .send = &send20};

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

        // Update offset algorithm
        if (imu->settings.gyroscopeOffsetEnabled) {
            gyroscope = FusionOffsetUpdate(&imu->offset, gyroscope);
        }

        // Axis alignment
        gyroscope = FusionAxesSwap(gyroscope, imu->settings.axisAlignment);
        accelerometer = FusionAxesSwap(accelerometer, imu->settings.axisAlignment);

        // Send inertial data
        const SendInertialData inertialData = {
            .ticks = icmData.ticks,
            .gyroscope = gyroscope,
            .accelerometer = accelerometer,
        };
        SendInertial(imu->send, &inertialData);

        // Send temperature data
        const SendTemperatureData temperatureData = {
            .ticks = icmData.ticks,
            .temperature = icmData.temperature,
        };
        SendTemperature(imu->send, &temperatureData);

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

        // Update AHRS algorithm
        FusionAhrsUpdateNoMagnetometer(&imu->ahrs, gyroscope, accelerometer, deltaTime);

        // AHRS callback
        const SendAhrsData ahrsData = {
            .ticks = icmData.ticks,
            .ahrs = &imu->ahrs,
        };
        SendAhrs(imu->send, &ahrsData);
    }
}

/**
 * @brief Sets the settings.
 * @param imu IMU structure.
 * @param settings Settings.
 */
void ImuSetSettings(Imu * const imu, const ImuSettings * const settings) {

    // Initialise offset
    if ((imu->initialised == false) || (imu->settings.sampleRate != settings->sampleRate)) {
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
