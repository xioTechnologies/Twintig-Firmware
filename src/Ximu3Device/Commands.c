/**
 * @file Commands.c
 * @author Seb Madgwick
 * @brief Commands.
 */

//------------------------------------------------------------------------------
// Includes

#include "Commands.h"
#include "Context.h"
#include "x-IMU3-Device/Ximu3Settings.h"

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
    // TODO: apply settings
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
    Context * const context_ = context;
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
    Context * const context_ = context;
    Ximu3CommandRespondPing(response, Ximu3SettingsGet(context_->settings)->deviceName, Ximu3SettingsGet(context_->settings)->serialNumber);
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
    Context * const context_ = context;
    return context_->factoryMode;
}

//------------------------------------------------------------------------------
// End of file
