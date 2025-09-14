/**
 * @file Ximu3Settings.c
 * @author Seb Madgwick
 * @brief x-IMU3 settings.
 */

//------------------------------------------------------------------------------
// Includes

#include <ctype.h>
#include <math.h>
#include "Metadata.h"
#include <string.h>
#include "Ximu3Settings.h"

//------------------------------------------------------------------------------
// Function declarations

static void SetValue(const Metadata * const metadata, const void* const value);
static void CopyString(char* const destination, const size_t destinationSize, const char* string);
static bool IsNanOrInf(const float value);

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
    for (int index = 0; index < XIMU3_NUMBER_OF_SETTINGS; index++) {
        const Metadata metadata = MetadataGet(settings, index);
        SetValue(&metadata, metadata.value);
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
void Ximu3SettingsDefaults(Ximu3Settings * const settings, const bool overwritePreserved) {

    // Loads defaults
    for (size_t index = 0; index < XIMU3_NUMBER_OF_SETTINGS; index++) {
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
    if ((metadata.type == MetadataTypeCharArray) && (strncmp(metadata.value, value, metadata.size) == 0)) {
        return;
    } else if (memcmp(metadata.value, value, metadata.size) == 0) {
        return;
    }

    // Clear applied flag
    *metadata.applied = false;

    // Write value
    SetValue(&metadata, value);
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
        case MetadataTypeCharArray:
            CopyString(metadata->value, metadata->size, value);
            return;
        case MetadataTypeFloat:
            if (IsNanOrInf(*((float*) value))) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFusionMatrix:
            if (IsNanOrInf(((FusionMatrix *) value)->element.xx) || IsNanOrInf(((FusionMatrix *) value)->element.xx) || IsNanOrInf(((FusionMatrix *) value)->element.xx) ||
                IsNanOrInf(((FusionMatrix *) value)->element.yy) || IsNanOrInf(((FusionMatrix *) value)->element.yy) || IsNanOrInf(((FusionMatrix *) value)->element.yy) ||
                IsNanOrInf(((FusionMatrix *) value)->element.zz) || IsNanOrInf(((FusionMatrix *) value)->element.zz) || IsNanOrInf(((FusionMatrix *) value)->element.zz)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFusionVector:
            if (IsNanOrInf(((FusionVector *) value)->axis.x) || IsNanOrInf(((FusionVector *) value)->axis.y) || IsNanOrInf(((FusionVector *) value)->axis.z)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFusionAxesAlignment:
            switch (*(FusionAxesAlignment*) value) {
                case FusionAxesAlignmentPXPYPZ:
                case FusionAxesAlignmentPXNZPY:
                case FusionAxesAlignmentPXNYNZ:
                case FusionAxesAlignmentPXPZNY:
                case FusionAxesAlignmentNXPYNZ:
                case FusionAxesAlignmentNXPZPY:
                case FusionAxesAlignmentNXNYPZ:
                case FusionAxesAlignmentNXNZNY:
                case FusionAxesAlignmentPYNXPZ:
                case FusionAxesAlignmentPYNZNX:
                case FusionAxesAlignmentPYPXNZ:
                case FusionAxesAlignmentPYPZPX:
                case FusionAxesAlignmentNYPXPZ:
                case FusionAxesAlignmentNYNZPX:
                case FusionAxesAlignmentNYNXNZ:
                case FusionAxesAlignmentNYPZNX:
                case FusionAxesAlignmentPZPYNX:
                case FusionAxesAlignmentPZPXPY:
                case FusionAxesAlignmentPZNYPX:
                case FusionAxesAlignmentPZNXNY:
                case FusionAxesAlignmentNZPYPX:
                case FusionAxesAlignmentNZNXPY:
                case FusionAxesAlignmentNZNYNX:
                case FusionAxesAlignmentNZPXNY:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeFusionConvention:
            switch (*(FusionConvention*) value) {
                case FusionConventionNwu:
                case FusionConventionEnu:
                case FusionConventionNed:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeImuAntiAliasing:
            switch (*(ImuAntiAliasing*) value) {
                case ImuAntiAliasingDisabled:
                case ImuAntiAliasing42Hz:
                case ImuAntiAliasing84Hz:
                case ImuAntiAliasing126Hz:
                case ImuAntiAliasing170Hz:
                case ImuAntiAliasing213Hz:
                case ImuAntiAliasing258Hz:
                case ImuAntiAliasing303Hz:
                case ImuAntiAliasing348Hz:
                case ImuAntiAliasing394Hz:
                case ImuAntiAliasing441Hz:
                case ImuAntiAliasing488Hz:
                case ImuAntiAliasing536Hz:
                case ImuAntiAliasing585Hz:
                case ImuAntiAliasing634Hz:
                case ImuAntiAliasing684Hz:
                case ImuAntiAliasing734Hz:
                case ImuAntiAliasing785Hz:
                case ImuAntiAliasing837Hz:
                case ImuAntiAliasing890Hz:
                case ImuAntiAliasing943Hz:
                case ImuAntiAliasing997Hz:
                case ImuAntiAliasing1051Hz:
                case ImuAntiAliasing1107Hz:
                case ImuAntiAliasing1163Hz:
                case ImuAntiAliasing1220Hz:
                case ImuAntiAliasing1277Hz:
                case ImuAntiAliasing1336Hz:
                case ImuAntiAliasing1395Hz:
                case ImuAntiAliasing1454Hz:
                case ImuAntiAliasing1515Hz:
                case ImuAntiAliasing1577Hz:
                case ImuAntiAliasing1639Hz:
                case ImuAntiAliasing1702Hz:
                case ImuAntiAliasing1766Hz:
                case ImuAntiAliasing1830Hz:
                case ImuAntiAliasing1896Hz:
                case ImuAntiAliasing1962Hz:
                case ImuAntiAliasing2029Hz:
                case ImuAntiAliasing2097Hz:
                case ImuAntiAliasing2166Hz:
                case ImuAntiAliasing2235Hz:
                case ImuAntiAliasing2306Hz:
                case ImuAntiAliasing2377Hz:
                case ImuAntiAliasing2449Hz:
                case ImuAntiAliasing2522Hz:
                case ImuAntiAliasing2596Hz:
                case ImuAntiAliasing2671Hz:
                case ImuAntiAliasing2746Hz:
                case ImuAntiAliasing2823Hz:
                case ImuAntiAliasing2900Hz:
                case ImuAntiAliasing2978Hz:
                case ImuAntiAliasing3057Hz:
                case ImuAntiAliasing3137Hz:
                case ImuAntiAliasing3217Hz:
                case ImuAntiAliasing3299Hz:
                case ImuAntiAliasing3381Hz:
                case ImuAntiAliasing3464Hz:
                case ImuAntiAliasing3548Hz:
                case ImuAntiAliasing3633Hz:
                case ImuAntiAliasing3718Hz:
                case ImuAntiAliasing3805Hz:
                case ImuAntiAliasing3892Hz:
                case ImuAntiAliasing3979Hz:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeImuSampleRate:
            switch (*(ImuSampleRate*) value) {
                case ImuSampleRate32kHz:
                case ImuSampleRate16kHz:
                case ImuSampleRate8kHz:
                case ImuSampleRate4kHz:
                case ImuSampleRate2kHz:
                case ImuSampleRate1kHz:
                case ImuSampleRate500Hz:
                case ImuSampleRate200Hz:
                case ImuSampleRate100Hz:
                case ImuSampleRate50Hz:
                case ImuSampleRate25Hz:
                case ImuSampleRate12Hz:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeSendAhrsMessageType:
            switch (*(SendAhrsMessageType*) value) {
                case SendAhrsMessageTypeQuaternion:
                case SendAhrsMessageTypeRotationMatrix:
                case SendAhrsMessageTypeEulerAngles:
                case SendAhrsMessageTypeLinearAcceleration:
                case SendAhrsMessageTypeEarthAcceleration:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
    }
    memcpy(metadata->value, metadata->defaultValue, metadata->size);
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
        if ((*string < 0) || (isprint(*string) == 0)) {
            destination[index] = '?';
        } else {
            destination[index] = *string;
        }
        string++;
    }
    destination[destinationSize - 1] = '\0';
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
 * @brief Saves to NVM.
 * @param settings Settings.
 */
void Ximu3SettingsSave(const Ximu3Settings * const settings) {
    if (settings->nvmWrite != NULL) {
        settings->nvmWrite(&settings->values, sizeof (settings->values), settings->context);
    }
}

/**
 * @brief Returns true if apply pending. Calling this function will reset the
 * flag.
 * @param settings Settings.
 * @param index Index.
 * @return True if apply pending.
 */
bool Ximu3SettingsApplyPending(Ximu3Settings * const settings, const Ximu3SettingsIndex index) {
    const Metadata metadata = MetadataGet(settings, index);
    const bool applied = *metadata.applied;
    *metadata.applied = true;
    return applied == false;
}

//------------------------------------------------------------------------------
// End of file
