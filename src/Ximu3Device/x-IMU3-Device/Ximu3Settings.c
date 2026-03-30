/**
 * @file Ximu3Settings.c
 * @author Seb Madgwick
 * @brief x-IMU3 settings.
 */

//------------------------------------------------------------------------------
// Includes

#include "Imu/Fusion/Fusion.h"
#include "Imu/Icm/Icm.h"
#include <math.h>
#include "Metadata.h"
#include "Send/Send.h"
#include <string.h>
#include "Ximu3Settings.h"

//------------------------------------------------------------------------------
// Function declarations

static void SetValue(const Metadata * const metadata, const void* const value);
static bool IsNanOrInf(const float value);
static void CopyString(char* const destination, const size_t destinationSize, const char* string);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module. This function must only be called once, on
 * system startup.
 * @param settings Settings.
 */
void Ximu3SettingsInitialise(Ximu3Settings * const settings) {

    // Read values from NVM
    if (settings->nvmRead != NULL) {
        settings->nvmRead(&settings->values, sizeof (settings->values), settings->context);
    } else {
        memset(&settings->values, 0xFF, sizeof (settings->values));
    }

    // Fix invalid values
    for (int integer = 0; integer < XIMU3_NUMBER_OF_SETTINGS; integer++) {
        Ximu3SettingsIndex index;
        if (Ximu3SettingsIndexFrom(&index, integer) != Ximu3ResultOk) {
            continue;
        }
        const Metadata metadata = MetadataGet(settings, index);
        SetValue(&metadata, metadata.value);
    }

    // Sanitise
    if (settings->sanitise != NULL) {
        for (int integer = 0; integer < XIMU3_NUMBER_OF_SETTINGS; integer++) {
            Ximu3SettingsIndex index;
            if (Ximu3SettingsIndexFrom(&index, integer) != Ximu3ResultOk) {
                continue;
            }
            settings->sanitise(index, settings->context);
        }
    }

    // Epilogue
    if (settings->initialiseEpilogue != NULL) {
        settings->initialiseEpilogue(settings->context);
    }
}

/**
 * @brief Loads defaults.
 * @param settings Settings.
 * @param overwritePreserved True to overwrite preserved settings.
 */
void Ximu3SettingsLoadDefaults(Ximu3Settings * const settings, const bool overwritePreserved) {

    // Load defaults
    for (int integer = 0; integer < XIMU3_NUMBER_OF_SETTINGS; integer++) {
        Ximu3SettingsIndex index;
        if (Ximu3SettingsIndexFrom(&index, integer) != Ximu3ResultOk) {
            continue;
        }
        const Metadata metadata = MetadataGet(settings, index);
        if (metadata.preserved && (overwritePreserved == false)) {
            continue;
        }
        Ximu3SettingsSet(settings, index, metadata.defaultValue, true);
    }

    // Epilogue
    if (settings->defaultsEpilogue != NULL) {
        settings->defaultsEpilogue(settings->context);
    }
}

/**
 * @brief Returns values.
 * @param settings Settings.
 * @return Values.
 */
const Ximu3SettingsValues* Ximu3SettingsGet(const Ximu3Settings * const settings) {
    return &settings->values;
}

/**
 * @brief Sets value.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 */
void Ximu3SettingsSet(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const void* const value, const bool overrideReadOnly) {

    // Get metadata
    const Metadata metadata = MetadataGet(settings, index);

    // Do nothing if read-only
    if ((overrideReadOnly == false) && metadata.readOnly) {
        return;
    }

    // Do nothing if value unchanged
    if ((metadata.type == MetadataTypeString) && (strncmp(metadata.value, value, metadata.size) == 0)) {
        return;
    } else if (memcmp(metadata.value, value, metadata.size) == 0) {
        return;
    }

    // Clear applied flag
    *metadata.applied = false;

    // Set value
    SetValue(&metadata, value);

    // Sanitise
    if (settings->sanitise != NULL) {
        settings->sanitise(index, settings->context);
    }
}

/**
 * @brief Sets value. Invalid values (including unterminated strings) will be
 * fixed.
 * @param metadata Metadata.
 * @param value Value.
 */
static void SetValue(const Metadata * const metadata, const void* const value) {

    // Set value
    switch (metadata->type) {
        case MetadataTypeBool:
        case MetadataTypeUint32:
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFloat:
            if (IsNanOrInf(*(float*) value)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFusionConvention:
            switch (*(FusionConvention*) value) {
                case FusionConventionNwu:
                case FusionConventionEnu:
                case FusionConventionNed:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeFusionMatrix:
            if (IsNanOrInf(((FusionMatrix*) value)->element.xx) || IsNanOrInf(((FusionMatrix*) value)->element.xy) || IsNanOrInf(((FusionMatrix*) value)->element.xz) ||
                IsNanOrInf(((FusionMatrix*) value)->element.yx) || IsNanOrInf(((FusionMatrix*) value)->element.yy) || IsNanOrInf(((FusionMatrix*) value)->element.yz) ||
                IsNanOrInf(((FusionMatrix*) value)->element.zx) || IsNanOrInf(((FusionMatrix*) value)->element.zy) || IsNanOrInf(((FusionMatrix*) value)->element.zz)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFusionRemapAlignment:
            switch (*(FusionRemapAlignment*) value) {
                case FusionRemapAlignmentPXPYPZ:
                case FusionRemapAlignmentPXPZNY:
                case FusionRemapAlignmentPXNZPY:
                case FusionRemapAlignmentPXNYNZ:
                case FusionRemapAlignmentPYPXNZ:
                case FusionRemapAlignmentPYPZPX:
                case FusionRemapAlignmentPYNZNX:
                case FusionRemapAlignmentPYNXPZ:
                case FusionRemapAlignmentPZPXPY:
                case FusionRemapAlignmentPZPYNX:
                case FusionRemapAlignmentPZNYPX:
                case FusionRemapAlignmentPZNXNY:
                case FusionRemapAlignmentNZPXNY:
                case FusionRemapAlignmentNZPYPX:
                case FusionRemapAlignmentNZNYNX:
                case FusionRemapAlignmentNZNXPY:
                case FusionRemapAlignmentNYPXPZ:
                case FusionRemapAlignmentNYPZNX:
                case FusionRemapAlignmentNYNZPX:
                case FusionRemapAlignmentNYNXNZ:
                case FusionRemapAlignmentNXPYNZ:
                case FusionRemapAlignmentNXPZPY:
                case FusionRemapAlignmentNXNZNY:
                case FusionRemapAlignmentNXNYPZ:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeFusionVector:
            if (IsNanOrInf(((FusionVector*) value)->axis.x) || IsNanOrInf(((FusionVector*) value)->axis.y) || IsNanOrInf(((FusionVector*) value)->axis.z)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeIcmLowPassFilter:
            switch (*(IcmLowPassFilter*) value) {
                case IcmLowPassFilterBypass:
                case IcmLowPassFilter4:
                case IcmLowPassFilter8:
                case IcmLowPassFilter16:
                case IcmLowPassFilter32:
                case IcmLowPassFilter64:
                case IcmLowPassFilter128:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeIcmSampleRate:
            switch (*(IcmSampleRate*) value) {
                case IcmSampleRate12Hz:
                case IcmSampleRate25Hz:
                case IcmSampleRate50Hz:
                case IcmSampleRate100Hz:
                case IcmSampleRate200Hz:
                case IcmSampleRate400Hz:
                case IcmSampleRate800Hz:
                case IcmSampleRate1600Hz:
                case IcmSampleRate3200Hz:
                case IcmSampleRate6400Hz:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeSendDataMessageMode:
            switch (*(SendDataMessageMode*) value) {
                case SendDataMessageModeBinary:
                case SendDataMessageModeAscii:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeSendInterfaceMode:
            switch (*(SendInterfaceMode*) value) {
                case SendInterfaceModeDisabled:
                case SendInterfaceModeBlocking:
                case SendInterfaceModeNonBlocking:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeSendOrientationMode:
            switch (*(SendOrientationMode*) value) {
                case SendOrientationModeQuaternion:
                case SendOrientationModeRotationMatrix:
                case SendOrientationModeEulerAngles:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeString:
            CopyString(metadata->value, metadata->size, value);
            return;
    }
    memcpy(metadata->value, metadata->defaultValue, metadata->size);
}

/**
 * @brief Returns true if NaN or Inf.
 * @param value Value.
 * @return True if NaN or Inf.
 */
static bool IsNanOrInf(const float value) {
    return isnan(value) || isinf(value);
}

/**
 * @brief Copies string. Unprintable characters are replaced with '?'. The
 * destination is padded with trailing zeros.
 * @param destination Destination.
 * @param destinationSize Destination size.
 * @param string String.
 */
static void CopyString(char* const destination, const size_t destinationSize, const char* string) {
    for (size_t index = 0; index < destinationSize; index++) {
        if (*string == '\0') {
            destination[index] = *string;
            continue;
        }
        if (((unsigned char) *string < 0x20) || ((unsigned char) *string > 0x7E)) {
            destination[index] = '?';
        } else {
            destination[index] = *string;
        }
        string++;
    }
    destination[destinationSize - 1] = '\0';
}

/**
 * @brief Saves to NVM.
 * @param settings Settings.
 */
void Ximu3SettingsSave(const Ximu3Settings * const settings) {
    if (settings->nvmWrite != NULL) {
        settings->nvmWrite(&settings->values, sizeof (settings->values), settings->context);
    }
}

/**
 * @brief Returns true if apply pending.
 * @param settings Settings.
 * @param index Index.
 * @return True if apply pending.
 */
bool Ximu3SettingsApplyPending(Ximu3Settings * const settings, const Ximu3SettingsIndex index) {
    const Metadata metadata = MetadataGet(settings, index);
    return *metadata.applied == false;
}

/**
 * @brief Clears apply pending.
 * @param settings Settings.
 */
void Ximu3SettingsClearApplyPending(Ximu3Settings * const settings) {
    for (int integer = 0; integer < XIMU3_NUMBER_OF_SETTINGS; integer++) {
        Ximu3SettingsIndex index;
        if (Ximu3SettingsIndexFrom(&index, integer) != Ximu3ResultOk) {
            continue;
        }
        const Metadata metadata = MetadataGet(settings, index);
        *metadata.applied = true;
    }
}

//------------------------------------------------------------------------------
// End of file
