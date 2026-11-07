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

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Max write size.
 */
#define MAX_WRITE_SIZE (256)

//------------------------------------------------------------------------------
// Variables

Imu imuA = {.icm = &icm1, .send = &sendA};
Imu imuB = {.icm = &icm2, .send = &sendB};
Imu imuC = {.icm = &icm3, .send = &sendC};
Imu imuD = {.icm = &icm4, .send = &sendD};
Imu imuE = {.icm = &icm5, .send = &sendE};
Imu imuF = {.icm = &icm6, .send = &sendF};
Imu imuG = {.icm = &icm7, .send = &sendG};
Imu imuH = {.icm = &icm8, .send = &sendH};
Imu imuI = {.icm = &icm9, .send = &sendI};
Imu imuJ = {.icm = &icm10, .send = &sendJ};
Imu imuK = {.icm = &icm11, .send = &sendK};
Imu imuL = {.icm = &icm12, .send = &sendL};
Imu imuM = {.icm = &icm13, .send = &sendM};
Imu imuN = {.icm = &icm14, .send = &sendN};
Imu imuO = {.icm = &icm15, .send = &sendO};
Imu imuP = {.icm = &icm16, .send = &sendP};
Imu imuQ = {.icm = &icm17, .send = &sendQ};
Imu imuR = {.icm = &icm18, .send = &sendR};
Imu imuS = {.icm = &icm19, .send = &sendS};
Imu imuT = {.icm = &icm20, .send = &sendT};

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
    while (SendAvailable(imu->send, MAX_WRITE_SIZE)) {

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
        gyroscope = FusionModelInertial(gyroscope, imu->settings.gyroscopeMisalignment, imu->settings.gyroscopeSensitivity, imu->settings.gyroscopeOffset);
        accelerometer = FusionModelInertial(accelerometer, imu->settings.accelerometerMisalignment, imu->settings.accelerometerSensitivity, imu->settings.accelerometerOffset);

        // Update bias algorithm
        if (imu->settings.biasCorrection) {
            FusionBiasUpdate(&imu->bias, gyroscope);
            gyroscope = FusionBiasGetCorrectedGyroscope(&imu->bias);
        }

        // Axis alignment
        gyroscope = FusionRemap(gyroscope, imu->settings.axisRemap);
        accelerometer = FusionRemap(accelerometer, imu->settings.axisRemap);

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

        // Do nothing else if AHRS disabled
        if (imu->settings.ahrsUpdateRateDivisor == 0) {
            continue;
        }

        // Downsampling
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
            gyroscope = FusionVectorScale(imu->downsampledGyroscope, reciprocal);
            accelerometer = FusionVectorScale(imu->downsampledAccelerometer, reciprocal);
        }
        imu->downsampledCount = 0;

        // Update AHRS algorithm
        FusionAhrsUpdateRelative(&imu->ahrs, gyroscope, accelerometer);

        // Send AHRS data
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

    // Initialise algorithms
    if (imu->initialised == false) {
        FusionAhrsInitialise(&imu->ahrs);
        FusionBiasInitialise(&imu->bias);
    }

    // Restart AHRS algorithm if certain settings changed
    if ((imu->settings.axisRemap != settings->axisRemap) ||
        (imu->settings.axesConvention != settings->axesConvention) ||
        (imu->settings.ahrsUpdateRateDivisor != settings->ahrsUpdateRateDivisor)) {
        FusionAhrsRestart(&imu->ahrs);
    }

    // Store new settings
    imu->settings = *settings;

    // Configure AHRS algorithm
    if (imu->settings.ahrsUpdateRateDivisor > 0) {
        const FusionAhrsSettings ahrsSettings = {
            .sampleRate = imu->settings.sampleRate / (float) imu->settings.ahrsUpdateRateDivisor,
            .convention = imu->settings.axesConvention,
            .headingMode = FusionAhrsHeadingModeRelative,
            .gain = imu->settings.ahrsGain,
            .gyroscopeRange = imu->settings.overrangeRecovery ? 2000.0f : 0.0f,
            .accelerationRejection = imu->settings.accelerationRejection ? 10.0f : 0.0f,
            .magneticRejection = 0.0f,
            .rejectionTimeout = fusionAhrsDefaultSettings.rejectionTimeout,
            .anchorCutoff = fusionAhrsDefaultSettings.anchorCutoff,
            .anchorDuration = fusionAhrsDefaultSettings.anchorDuration,
        };
        FusionAhrsSetSettings(&imu->ahrs, &ahrsSettings);
    }

    // Configure bias algorithm
    const FusionBiasSettings biasSettings = {
        .sampleRate = imu->settings.sampleRate,
        .duration = fusionBiasDefaultSettings.duration,
        .threshold = fusionBiasDefaultSettings.threshold,
        .continuous = true,
        .holdoff = fusionBiasDefaultSettings.holdoff,
    };
    FusionBiasSetSettings(&imu->bias, &biasSettings);

    // Set flag
    imu->initialised = true;
}

/**
 * @brief Restarts the AHRS algorithm.
 * @param imu IMU structure.
 */
void ImuRestart(Imu * const imu) {
    FusionAhrsRestart(&imu->ahrs);
}

/**
 * @brief Sets the heading.
 * @param imu IMU structure.
 * @param heading Heading in degrees.
 */
void ImuSetHeading(Imu * const imu, const float heading) {
    FusionAhrsSetHeading(&imu->ahrs, heading);
}

//------------------------------------------------------------------------------
// End of file
