/**
 * @file Imu.c
 * @author Seb Madgwick
 * @brief IMU processing.
 */

//------------------------------------------------------------------------------
// Includes

#include "Imu.h"
#include <stddef.h>
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Variables

Imu imu1 = {.icm = &icm1};

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

    // Process avaliable data
    IcmData imuData;
    while (imu->icm->getData(&imuData) == IcmResultOK) {

        // Apply calibration
        FusionVector gyroscope = {
            .axis.x = imuData.gyroscopeX,
            .axis.y = imuData.gyroscopeY,
            .axis.z = imuData.gyroscopeZ,
        };
        FusionVector accelerometer = {
            .axis.x = imuData.accelerometerX,
            .axis.y = imuData.accelerometerY,
            .axis.z = imuData.accelerometerZ,
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
                .timestamp = imuData.timestamp,
                .gyroscope = gyroscope,
                .accelerometer = accelerometer,
            };
            imu->inertialDataReady(&inertialData, imu->context);
        }

        // Temperature callback
        if (imu->temperatureDataReady != NULL) {
            ImuTemperatureData temperatureData = {
                .timestamp = imuData.timestamp,
                .temperature = imuData.temperature,
            };
            imu->temperatureDataReady(&temperatureData, imu->context);
        }

        // Downsampling
        if (imu->settings.ahrsUpdateRateDivisor == 0) {
            break;
        }
        if (imu->settings.ahrsUpdateRateDivisor > 1) {
            if (imu->downsampledCount == 0) {
                imu->downsampledGyroscope = FUSION_VECTOR_ZERO;
                imu->downsampledAccelerometer = FUSION_VECTOR_ZERO;
            }
            imu->downsampledGyroscope = FusionVectorAdd(imu->downsampledGyroscope, gyroscope);
            imu->downsampledAccelerometer = FusionVectorAdd(imu->downsampledAccelerometer, accelerometer);
            if (++imu->downsampledCount < imu->settings.ahrsUpdateRateDivisor) {
                break;
            }
            const float reciprocal = 1.0f / (float) imu->downsampledCount;
            gyroscope = FusionVectorMultiplyScalar(imu->downsampledGyroscope, reciprocal);
            accelerometer = FusionVectorMultiplyScalar(imu->downsampledAccelerometer, reciprocal);
        }
        imu->downsampledCount = 0;

        // Calculate delta time
        const bool invalid = imu->previousTimestamp == 0;
        const float deltaTime = (float) ((double) (imuData.timestamp - imu->previousTimestamp) * (1.0 / (double) TIMER_TICKS_PER_SECOND));
        imu->previousTimestamp = imuData.timestamp;
        if (invalid) {
            break;
        }

        // Update AHRS
        FusionAhrsUpdateNoMagnetometer(&imu->ahrs, gyroscope, accelerometer, deltaTime);

        // AHRS callback
        if (imu->ahrsDataReady != NULL) {
            ImuAhrsData ahrsData = {
                .timestamp = imuData.timestamp,
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
        imu->icm->initialise(settings->sampleRate);
        FusionOffsetInitialise(&imu->offset, (int) (IcmOdrToFloat(settings->sampleRate) + 0.5f));
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
