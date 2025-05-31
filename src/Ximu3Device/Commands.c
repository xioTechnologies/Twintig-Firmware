/**
 * @file Commands.c
 * @author Seb Madgwick
 * @brief Commands.
 */

//------------------------------------------------------------------------------
// Includes

#include "Apply.h"
#include "Commands.h"
#include "Context.h"
#include "Haptic/Haptic.h"
#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Default command.
 * @param value Value.
 * @param response Response.
 */
void CommandsDefault(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    Context * const context_ = context;
    Ximu3SettingsDefaults(context_->settings, context_->factoryMode);
    ApplyAfterDelay(context_);
    Ximu3CommandRespond(response);
}

/**
 * @brief Apply command.
 * @param value Value.
 * @param response Response.
 */
void CommandsApply(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    Context * const context_ = context;
    ApplyNow(context_);
    Ximu3CommandRespond(response);
}

/**
 * @brief Save command.
 * @param value Value.
 * @param response Response.
 */
void CommandsSave(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    const Context * const context_ = context;
    if (context_->nvmBlank && (context_->factoryMode == false)) {
        Ximu3CommandRespondError(response, "NVM blank");
        return;
    }
    Ximu3SettingsSave(context_->settings);
    Ximu3CommandRespond(response);
}

/**
 * @brief Ping command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsPing(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    const Context * const context_ = context;
    Ximu3CommandRespondPing(response, Ximu3SettingsGet(context_->settings)->deviceName, Ximu3SettingsGet(context_->settings)->serialNumber);
}

/**
 * @brief Haptic command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsHaptic(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    float id;
    if (Ximu3CommandParseNumber(value, response, &id) != 0) {
        return;
    }
    if (HapticPlay((int) id) != HapticResultOk) {
        Ximu3CommandRespondError(response, "Invalid ID");
        return;
    }
    Ximu3CommandRespond(response);
}

/**
 * @brief Initialise command.
 * @param value Value.
 * @param response Response.
 */
void CommandsInitialise(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    const Context * const context_ = context;
    ImuReset(context_->imu);
    Ximu3CommandRespond(response);
}

/**
 * @brief Heading command.
 * @param value Value.
 * @param response Response.
 */
void CommandsHeading(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    float heading;
    if (Ximu3CommandParseNumber(value, response, &heading) != 0) {
        return;
    }
    const Context * const context_ = context;
    ImuSetHeading(context_->imu, heading);
    Ximu3CommandRespond(response);
}

/**
 * @brief Note command.
 * @param value Value.
 * @param response Response.
 */
void CommandsNote(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    char string[XIMU3_VALUE_SIZE];
    if (Ximu3CommandParseString(value, response, string, sizeof (string), NULL) != 0) {
        return;
    }
    const Context * const context_ = context;
    SendNotification(context_->send, string);
    Ximu3CommandRespond(response);
}

/**
 * @brief Factory command.
 * @param value Value.
 * @param response Response.
 */
void CommandsFactory(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    Context * const context_ = context;
    context_->factoryMode = true;
    Ximu3CommandRespond(response);
}

/**
 * @brief Returns true if factory mode enabled.
 * @return True if factory mode enabled.
 */
bool CommandsOverrideReadOnly(void* const context) {
    const Context * const context_ = context;
    return context_->factoryMode;
}

//------------------------------------------------------------------------------
// End of file
