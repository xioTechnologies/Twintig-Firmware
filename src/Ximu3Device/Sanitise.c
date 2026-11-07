/**
 * @file Sanitise.c
 * @author Seb Madgwick
 * @brief Sanitise settings.
 */

//------------------------------------------------------------------------------
// Includes

#include "Context.h"
#include <float.h>
#include "Sanitise.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Function declarations

static void ClampFloat(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const float value, const float min, const float max);
static void ClampUint32(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const uint32_t value, const uint32_t min, const uint32_t max);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Sanitises settings.
 * @param index Index.
 * @param context Context.
 */
void Sanitise(const Ximu3SettingsIndex index, void* const context) {
    Context * const context_ = context;
    const Ximu3SettingsValues * const settingsValues = Ximu3SettingsGet(context_->settings);
    switch (index) {
        case Ximu3SettingsIndexBaudRate:
            ClampUint32(context_->settings, index, settingsValues->baudRate, 400, 25000000);
            break;
        case Ximu3SettingsIndexAhrsGain:
            ClampFloat(context_->settings, index, settingsValues->ahrsGain, 0.0f, FLT_MAX);
            break;
        default:
            break;
    }
}

/**
 * @brief Clamps a float.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param min Minimum value.
 * @param max Maximum value.
 */
static void ClampFloat(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const float value, const float min, const float max) {
    if (value < min) {
        Ximu3SettingsSet(settings, index, &min, false);
    }
    if (value > max) {
        Ximu3SettingsSet(settings, index, &max, false);
    }
}

/**
 * @brief Clamps a uint32_t.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param min Minimum value.
 * @param max Maximum value.
 */
static void ClampUint32(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const uint32_t value, const uint32_t min, const uint32_t max) {
    if (value < min) {
        Ximu3SettingsSet(settings, index, &min, false);
    }
    if (value > max) {
        Ximu3SettingsSet(settings, index, &max, false);
    }
}

//------------------------------------------------------------------------------
// End of file
