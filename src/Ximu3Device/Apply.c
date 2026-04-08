/**
 * @file Apply.c
 * @author Seb Madgwick
 * @brief Applies settings.
 */

//------------------------------------------------------------------------------
// Includes

#include "Apply.h"
#include "Imu/Imu.h"
#include "Send/Send.h"
#include "Serial/Serial.h"
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Function declarations

static void ApplySerial(Context * const context);
static void ApplyIcm(Context * const context);
static void ApplyImu(Context * const context);
static void ApplySend(Context * const context);

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
static void ApplySerial(Context * const context) {

    // Do nothing if not applicable
    if (context->isMain == false) {
        return;
    }

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialEnabled)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialBaudRate)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialRtsCtsEnabled)) == false) {
        return;
    }

    // Apply settings
    const SerialSettings serialSettings = {
        .enabled = Ximu3SettingsGet(context->settings)->serialEnabled,
        .baudRate = Ximu3SettingsGet(context->settings)->serialBaudRate,
        .rtsCtsEnabled = Ximu3SettingsGet(context->settings)->serialRtsCtsEnabled,
    };
    SerialSetSettings(&serialSettings);
}

/**
 * @brief Applies ICM settings.
 * @param context Context.
 */
static void ApplyIcm(Context * const context) {

    // Do nothing if not applicable
    if (context->imu == NULL) {
        return;
    }

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeNotchFilterEnabled)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeAntiAliasing)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerAntiAliasing)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSampleRate)) == false) {
        return;
    }

    // Apply settings
    const IcmSettings icmSettings = {
        .gyroscopeNotchFilterEnabled = Ximu3SettingsGet(context->settings)->gyroscopeNotchFilterEnabled,
        .gyroscopeAntiAliasing = Ximu3SettingsGet(context->settings)->gyroscopeAntiAliasing,
        .accelerometerAntiAliasing = Ximu3SettingsGet(context->settings)->accelerometerAntiAliasing,
        .sampleRate = Ximu3SettingsGet(context->settings)->sampleRate,
    };
    context->imu->icm->initialise(&icmSettings);
}

/**
 * @brief Applies IMU settings.
 * @param context Context.
 */
static void ApplyImu(Context * const context) {

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
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAxesRemap)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeBiasCorrectionEnabled)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsUpdateRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsAxesConvention)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsGain)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsAccelerationRejection)) == false) {
        return;
    }

    // Apply settings
    const ImuSettings imuSettings = {
        .gyroscopeMisalignment = Ximu3SettingsGet(context->settings)->gyroscopeMisalignment,
        .gyroscopeSensitivity = Ximu3SettingsGet(context->settings)->gyroscopeSensitivity,
        .gyroscopeOffset = Ximu3SettingsGet(context->settings)->gyroscopeOffset,
        .accelerometerMisalignment = Ximu3SettingsGet(context->settings)->accelerometerMisalignment,
        .accelerometerSensitivity = Ximu3SettingsGet(context->settings)->accelerometerSensitivity,
        .accelerometerOffset = Ximu3SettingsGet(context->settings)->accelerometerOffset,
        .sampleRate = Ximu3SettingsGet(context->settings)->sampleRate,
        .axesRemap = Ximu3SettingsGet(context->settings)->axesRemap,
        .gyroscopeBiasCorrectionEnabled = Ximu3SettingsGet(context->settings)->gyroscopeBiasCorrectionEnabled,
        .ahrsUpdateRateDivisor = Ximu3SettingsGet(context->settings)->ahrsUpdateRateDivisor,
        .ahrsAxesConvention = Ximu3SettingsGet(context->settings)->ahrsAxesConvention,
        .ahrsGain = Ximu3SettingsGet(context->settings)->ahrsGain,
        .ahrsAccelerationRejection = Ximu3SettingsGet(context->settings)->ahrsAccelerationRejection,
    };
    ImuSetSettings(context->imu, &imuSettings);
}

/**
 * @brief Applies send settings.
 * @param context Context.
 */
static void ApplySend(Context * const context) {

    // Do nothing if settings unchanged
    if ((Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexDataMessageMode)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsMessageType)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexInertialMessageRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsMessageRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexTemperatureMessageRateDivisor)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexUsbSendMode)
        || Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialSendMode)) == false) {
        return;
    }

    // Apply settings
    const SendSettings sendSettings = {
        .dataMessageMode = Ximu3SettingsGet(context->settings)->dataMessageMode,
        .ahrsMessageType = Ximu3SettingsGet(context->settings)->ahrsMessageType,
        .inertialMessageRateDivisor = Ximu3SettingsGet(context->settings)->inertialMessageRateDivisor,
        .ahrsMessageRateDivisor = Ximu3SettingsGet(context->settings)->ahrsMessageRateDivisor,
        .temperatureMessageRateDivisor = Ximu3SettingsGet(context->settings)->temperatureMessageRateDivisor,
        .usbSendMode = Ximu3SettingsGet(context->settings)->usbSendMode,
        .serialSendMode = Ximu3SettingsGet(context->settings)->serialSendMode,
    };
    SendSetSettings(context->send, &sendSettings);
}

//------------------------------------------------------------------------------
// End of file
