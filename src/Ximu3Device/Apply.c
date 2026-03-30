/**
 * @file Apply.c
 * @author Seb Madgwick
 * @brief Applies settings.
 */

//------------------------------------------------------------------------------
// Includes

#include "Apply.h"
#include "Imu/Icm/Icm.h"
#include "Imu/Imu.h"
#include "Send/Send.h"
#include "Serial/Serial.h"
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Function declarations

static void ApplySerial(const Context * const context);
static void ApplyIcm(const Context * const context);
static void ApplyImu(const Context * const context);
static void ApplySend(const Context * const context);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 * @param context Context.
 */
void ApplyTasks(Context * const context) {
    if ((context->applyTimeout != 0) && (TimerGetTicks64() > context->applyTimeout)) {
        ApplyNow(context);
    }
}

/**
 * @brief Applies settings immediately.
 * @param context Context.
 */
void ApplyNow(Context * const context) {
    context->applyTimeout = 0;
    ApplySerial(context);
    ApplyIcm(context);
    ApplyImu(context);
    ApplySend(context);
    Ximu3SettingsClearApplyPending(context->settings);
}

/**
 * @brief Applies settings after a delay.
 * @param context Context.
 */
void ApplyAfterDelay(Context * const context) {
    context->applyTimeout = TimerGetTicks64() + (2 * TIMER_TICKS_PER_SECOND);
}

/**
 * @brief Applies serial settings.
 * @param context Context.
 */
static void ApplySerial(const Context * const context) {

    // Do nothing if not applicable
    if (context->isMain == false) {
        return;
    }

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialEnabled)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexBaudRate)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexRtsCts)) == false) {
        return;
    }

    // Apply settings
    const Ximu3SettingsValues * const settingsValues = Ximu3SettingsGet(context->settings);
    const SerialSettings serialSettings = {
        .enabled = settingsValues->serialEnabled,
        .baudRate = settingsValues->baudRate,
        .rtsCts = settingsValues->rtsCts,
    };
    SerialSetSettings(&serialSettings);
}

/**
 * @brief Applies ICM settings.
 * @param context Context.
 */
static void ApplyIcm(const Context * const context) {

    // Do nothing if not applicable
    if (context->imu == NULL) {
        return;
    }

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSampleRate)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeLowPassFilter)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerLowPassFilter)) == false) {
        return;
    }

    // Apply settings
    const Ximu3SettingsValues * const settingsValues = Ximu3SettingsGet(context->settings);
    const IcmSettings icmSettings = {
        .sampleRate = settingsValues->sampleRate,
        .gyroscopeLowPassFilter = settingsValues->gyroscopeLowPassFilter,
        .accelerometerLowPassFilter = settingsValues->accelerometerLowPassFilter,
    };
    context->imu->icm->initialise(&icmSettings);
}

/**
 * @brief Applies IMU settings.
 * @param context Context.
 */
static void ApplyImu(const Context * const context) {

    // Do nothing if not applicable
    if (context->imu == NULL) {
        return;
    }

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeMisalignment)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeSensitivity)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeOffset)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerMisalignment)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerSensitivity)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerOffset)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSampleRate)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAxisRemap)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexBiasCorrection)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsUpdateRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAxesConvention)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsGain)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexOverrangeRecovery)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerationRejection)) == false) {
        return;
    }

    // Apply settings
    const Ximu3SettingsValues * const settingsValues = Ximu3SettingsGet(context->settings);
    const ImuSettings imuSettings = {
        .gyroscopeMisalignment = settingsValues->gyroscopeMisalignment,
        .gyroscopeSensitivity = settingsValues->gyroscopeSensitivity,
        .gyroscopeOffset = settingsValues->gyroscopeOffset,
        .accelerometerMisalignment = settingsValues->accelerometerMisalignment,
        .accelerometerSensitivity = settingsValues->accelerometerSensitivity,
        .accelerometerOffset = settingsValues->accelerometerOffset,
        .sampleRate = settingsValues->sampleRate,
        .axisRemap = settingsValues->axisRemap,
        .biasCorrection = settingsValues->biasCorrection,
        .ahrsUpdateRateDivisor = settingsValues->ahrsUpdateRateDivisor,
        .axesConvention = settingsValues->axesConvention,
        .ahrsGain = settingsValues->ahrsGain,
        .overrangeRecovery = settingsValues->overrangeRecovery,
        .accelerationRejection = settingsValues->accelerationRejection,
    };
    ImuSetSettings(context->imu, &imuSettings);
}

/**
 * @brief Applies send settings.
 * @param context Context.
 */
static void ApplySend(const Context * const context) {

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexDataMessageMode)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexOrientationMode)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexInertialMessageRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexOrientationMessageRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexTemperatureMessageRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexUsbSendMode)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialSendMode)) == false) {
        return;
    }

    // Apply settings
    const Ximu3SettingsValues * const settingsValues = Ximu3SettingsGet(context->settings);
    const SendSettings sendSettings = {
        .dataMessageMode = settingsValues->dataMessageMode,
        .orientationMode = settingsValues->orientationMode,
        .inertialMessageRateDivisor = settingsValues->inertialMessageRateDivisor,
        .orientationMessageRateDivisor = settingsValues->orientationMessageRateDivisor,
        .temperatureMessageRateDivisor = settingsValues->temperatureMessageRateDivisor,
        .usbSendMode = settingsValues->usbSendMode,
        .serialSendMode = settingsValues->serialSendMode,
    };
    SendSetSettings(context->send, &sendSettings);
}

//------------------------------------------------------------------------------
// End of file
