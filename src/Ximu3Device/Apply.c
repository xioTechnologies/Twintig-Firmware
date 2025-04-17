/**
 * @file Apply.c
 * @author Seb Madgwick
 * @brief Apply.
 */

//------------------------------------------------------------------------------
// Includes

#include "Apply.h"
#include "Timer/Timer.h"
#include "Uart/Uart1.h"
#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Function declarations

static void ApplySerial(Ximu3Settings * const settings);

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
    ApplySerial(context->settings);
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
 * @param settings Settings.
 */
void ApplySerial(Ximu3Settings * const settings) {

    // Do nothing if settings unchanged
    bool applyPending = false;
    applyPending |= Ximu3SettingsApplyPending(settings, Ximu3SettingsIndexSerialEnabled);
    applyPending |= Ximu3SettingsApplyPending(settings, Ximu3SettingsIndexSerialBaudRate);
    applyPending |= Ximu3SettingsApplyPending(settings, Ximu3SettingsIndexSerialRtsCtsEnabled);
    if (applyPending == false) {
        return;
    }

    // Apply settings
    if (Ximu3SettingsGet(settings)->serialEnabled) {
        UartSettings uartSettings = uartSettingsDefault;
        uartSettings.baudRate = Ximu3SettingsGet(settings)->serialBaudRate;
        uartSettings.rtsCtsEnabled = Ximu3SettingsGet(settings)->serialRtsCtsEnabled;
        Uart1Initialise(&uartSettings);
    } else {
        Uart1Deinitialise();
    }
}

//------------------------------------------------------------------------------
// End of file
