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
#include "Imu/Imu.h"
#include "Led/Led.h"
#include <stdint.h>
#include <stdio.h>
#include "Timestamp/Timestamp.h"
#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Default command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsDefault(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
        return;
    }
    Context * const context_ = context;
    Ximu3SettingsDefaults(context_->settings, false);
    ApplyAfterDelay(context_);
    Ximu3CommandRespond(response);
}

/**
 * @brief Apply command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsApply(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
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
 * @param context Context.
 */
void CommandsSave(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
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
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    Ximu3CommandRespondPing(response, Ximu3SettingsGet(context_->settings)->deviceName, Ximu3SettingsGet(context_->settings)->serialNumber);
}

/**
 * @brief Blink command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsBlink(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    if (LedBlink(context_->led, ledColourWhite) != LedResultOk) {
        Ximu3CommandRespondError(response, "LED override enabled");
        return;
    }
    Ximu3CommandRespond(response);
}

/**
 * @brief Strobe command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsStrobe(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    if (LedStrobe(context_->led) != LedResultOk) {
        Ximu3CommandRespondError(response, "LED override enabled");
        return;
    }
    Ximu3CommandRespond(response);
}

/**
 * @brief Colour command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsColour(const char* * const value, Ximu3CommandResponse * const response, void* const context) {

    // Parse null
    const Context * const context_ = context;
    if (JsonParseNull(value) == JsonResultOk) {
        LedDisableOverride(context_->led);
        Ximu3CommandRespond(response);
        return;
    }

    // Parse string
    char string[XIMU3_VALUE_SIZE];
    if (Ximu3CommandParseString(value, response, string, sizeof (string), NULL) != Ximu3ResultOk) {
        return;
    }

    // Parse RGB hex
    LedColour colour;
    if (sscanf(string, "#%X", &colour.rgb) != 1) {
        Ximu3CommandRespondError(response, "String format must be #RRGGBB");
        return;
    }

    // Override LED
    LedOverride(context_->led, colour);
    snprintf(response->value, sizeof (response->value), "\"#%06X\"", colour.rgb);
    Ximu3CommandRespond(response);
}

/**
 * @brief Haptic command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsHaptic(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    float id;
    if (Ximu3CommandParseNumber(value, response, &id) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    if (context_->hapticPlay != NULL) {
        if (context_->hapticPlay((int) id) != HapticResultOk) {
            Ximu3CommandRespondError(response, "Invalid ID");
            return;
        }
    }
    Ximu3CommandRespond(response);
}

/**
 * @brief Initialise command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsInitialise(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    if (context_->imu != NULL) {
        ImuReset(context_->imu);
    }
    Ximu3CommandRespond(response);
}

/**
 * @brief Heading command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsHeading(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    float heading;
    if (Ximu3CommandParseNumber(value, response, &heading) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    if (context_->imu != NULL) {
        ImuSetHeading(context_->imu, heading);
    }
    Ximu3CommandRespond(response);
}

/**
 * @brief Note command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsNote(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    char string[XIMU3_VALUE_SIZE];
    if (Ximu3CommandParseString(value, response, string, sizeof (string), NULL) != Ximu3ResultOk) {
        return;
    }
    const Context * const context_ = context;
    SendNotification(context_->send, string);
    Ximu3CommandRespond(response);
}

/**
 * @brief Timestamp command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsTimestamp(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    uint64_t timestamp;
    if (Ximu3CommandParseNumberU64(value, response, &timestamp) != Ximu3ResultOk) {
        return;
    }
    TimestampSet(timestamp);
    Ximu3CommandRespond(response);
}

/**
 * @brief Factory command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsFactory(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != Ximu3ResultOk) {
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
