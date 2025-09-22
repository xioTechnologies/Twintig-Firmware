/**
 * @file Apply.c
 * @author Seb Madgwick
 * @brief Apply.
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
    ApplyImu(context);
    ApplySend(context);
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
void ApplySerial(Context * const context) {

    // Do nothing if settings unchanged
    bool applyPending = false;
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialEnabled);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialBaudRate);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialRtsCtsEnabled);
    if (applyPending == false) {
        return;
    }

    // Apply settings
    if (context->serialSetSettings == NULL) {
        return;
    }
    const SerialSettings serialSettings = {
        .enabled = Ximu3SettingsGet(context->settings)->serialEnabled,
        .baudRate = Ximu3SettingsGet(context->settings)->serialBaudRate,
        .rtsCtsEnabled = Ximu3SettingsGet(context->settings)->serialRtsCtsEnabled,
    };
    context->serialSetSettings(&serialSettings);
}

/**
 * @brief Applies IMU settings.
 * @param context Context.
 */
static void ApplyImu(Context * const context) {

    // Do nothing if settings unchanged
    bool applyPending = false;
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexNotchFilterEnabled);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAntiAliasing);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSampleRate);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeMisalignment);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeSensitivity);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeOffset);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerMisalignment);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerSensitivity);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAccelerometerOffset);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAxesAlignment);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexGyroscopeOffsetCorrectionEnabled);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsUpdateRateDivisor);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsAxesConvention);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsGain);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsAccelerationRejection);
    if (applyPending == false) {
        return;
    }

    // Apply settings
    if (context->imu == NULL) {
        return;
    }
    const ImuSettings imuSettings = {
        .notchFilterEnabled = Ximu3SettingsGet(context->settings)->notchFilterEnabled,
        .antiAliasing = Ximu3SettingsGet(context->settings)->antiAliasing,
        .sampleRate = Ximu3SettingsGet(context->settings)->sampleRate,
        .gyroscopeMisalignment = Ximu3SettingsGet(context->settings)->gyroscopeMisalignment,
        .gyroscopeSensitivity = Ximu3SettingsGet(context->settings)->gyroscopeSensitivity,
        .gyroscopeOffset = Ximu3SettingsGet(context->settings)->gyroscopeOffset,
        .accelerometerMisalignment = Ximu3SettingsGet(context->settings)->accelerometerMisalignment,
        .accelerometerSensitivity = Ximu3SettingsGet(context->settings)->accelerometerSensitivity,
        .accelerometerOffset = Ximu3SettingsGet(context->settings)->accelerometerOffset,
        .axisAlignment = Ximu3SettingsGet(context->settings)->axesAlignment,
        .gyroscopeOffsetEnabled = Ximu3SettingsGet(context->settings)->gyroscopeOffsetCorrectionEnabled,
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
    bool applyPending = false;
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexBinaryModeEnabled);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsMessageType);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexInertialMessageRateDivisor);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexAhrsMessageRateDivisor);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexTemperatureMessageRateDivisor);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexUsbDataMessagesEnabled);
    applyPending |= Ximu3SettingsApplyPending(context->settings, Ximu3SettingsIndexSerialDataMessagesEnabled);
    if (applyPending == false) {
        return;
    }

    // Apply settings
    const SendSettings sendSettings = {
        .binaryModeEnabled = Ximu3SettingsGet(context->settings)->binaryModeEnabled,
        .ahrsMessageType = Ximu3SettingsGet(context->settings)->ahrsMessageType,
        .inertialMessageRateDivisor = Ximu3SettingsGet(context->settings)->inertialMessageRateDivisor,
        .ahrsMessageRateDivisor = Ximu3SettingsGet(context->settings)->ahrsMessageRateDivisor,
        .temperatureMessageRateDivisor = Ximu3SettingsGet(context->settings)->temperatureMessageRateDivisor,
        .usbDataMessagesEnabled = Ximu3SettingsGet(context->settings)->usbDataMessagesEnabled,
        .serialDataMessagesEnabled = Ximu3SettingsGet(context->settings)->serialDataMessagesEnabled,
    };
    SendSetSettings(context->send, &sendSettings, context->imu);
}

//------------------------------------------------------------------------------
// End of file
