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
 * @brief Load defaults.
 * @param settings Settings.
 * @param overwritePreserved True to overwrite preserved settings.
 */
void Ximu3SettingsLoadDefaults(Ximu3Settings * const settings, const bool overwritePreserved) {

    // Load defaults
    for (int index = 0; index < XIMU3_NUMBER_OF_SETTINGS; index++) {
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
    if ((metadata.type == MetadataTypeString) && (strncmp(metadata.value, value, metadata.size) == 0)) {
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
            if (IsNanOrInf(((FusionMatrix *) value)->element.xx) || IsNanOrInf(((FusionMatrix *) value)->element.xy) || IsNanOrInf(((FusionMatrix *) value)->element.xz) ||
                IsNanOrInf(((FusionMatrix *) value)->element.yx) || IsNanOrInf(((FusionMatrix *) value)->element.yy) || IsNanOrInf(((FusionMatrix *) value)->element.yz) ||
                IsNanOrInf(((FusionMatrix *) value)->element.zx) || IsNanOrInf(((FusionMatrix *) value)->element.zy) || IsNanOrInf(((FusionMatrix *) value)->element.zz)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeFusionRemapAlignment:
            switch (*(FusionRemapAlignment*) value) {
                case FusionRemapAlignmentPXPYPZ:
                case FusionRemapAlignmentPXNZPY:
                case FusionRemapAlignmentPXNYNZ:
                case FusionRemapAlignmentPXPZNY:
                case FusionRemapAlignmentNXPYNZ:
                case FusionRemapAlignmentNXPZPY:
                case FusionRemapAlignmentNXNYPZ:
                case FusionRemapAlignmentNXNZNY:
                case FusionRemapAlignmentPYNXPZ:
                case FusionRemapAlignmentPYNZNX:
                case FusionRemapAlignmentPYPXNZ:
                case FusionRemapAlignmentPYPZPX:
                case FusionRemapAlignmentNYPXPZ:
                case FusionRemapAlignmentNYNZPX:
                case FusionRemapAlignmentNYNXNZ:
                case FusionRemapAlignmentNYPZNX:
                case FusionRemapAlignmentPZPYNX:
                case FusionRemapAlignmentPZPXPY:
                case FusionRemapAlignmentPZNYPX:
                case FusionRemapAlignmentPZNXNY:
                case FusionRemapAlignmentNZPYPX:
                case FusionRemapAlignmentNZNXPY:
                case FusionRemapAlignmentNZNYNX:
                case FusionRemapAlignmentNZPXNY:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeFusionVector:
            if (IsNanOrInf(((FusionVector *) value)->axis.x) || IsNanOrInf(((FusionVector *) value)->axis.y) || IsNanOrInf(((FusionVector *) value)->axis.z)) {
                break;
            }
            memcpy(metadata->value, value, metadata->size);
            return;
        case MetadataTypeIcmAntiAliasing:
            switch (*(IcmAntiAliasing*) value) {
                case IcmAntiAliasingDisabled:
                case IcmAntiAliasing42Hz:
                case IcmAntiAliasing84Hz:
                case IcmAntiAliasing126Hz:
                case IcmAntiAliasing170Hz:
                case IcmAntiAliasing213Hz:
                case IcmAntiAliasing258Hz:
                case IcmAntiAliasing303Hz:
                case IcmAntiAliasing348Hz:
                case IcmAntiAliasing394Hz:
                case IcmAntiAliasing441Hz:
                case IcmAntiAliasing488Hz:
                case IcmAntiAliasing536Hz:
                case IcmAntiAliasing585Hz:
                case IcmAntiAliasing634Hz:
                case IcmAntiAliasing684Hz:
                case IcmAntiAliasing734Hz:
                case IcmAntiAliasing785Hz:
                case IcmAntiAliasing837Hz:
                case IcmAntiAliasing890Hz:
                case IcmAntiAliasing943Hz:
                case IcmAntiAliasing997Hz:
                case IcmAntiAliasing1051Hz:
                case IcmAntiAliasing1107Hz:
                case IcmAntiAliasing1163Hz:
                case IcmAntiAliasing1220Hz:
                case IcmAntiAliasing1277Hz:
                case IcmAntiAliasing1336Hz:
                case IcmAntiAliasing1395Hz:
                case IcmAntiAliasing1454Hz:
                case IcmAntiAliasing1515Hz:
                case IcmAntiAliasing1577Hz:
                case IcmAntiAliasing1639Hz:
                case IcmAntiAliasing1702Hz:
                case IcmAntiAliasing1766Hz:
                case IcmAntiAliasing1830Hz:
                case IcmAntiAliasing1896Hz:
                case IcmAntiAliasing1962Hz:
                case IcmAntiAliasing2029Hz:
                case IcmAntiAliasing2097Hz:
                case IcmAntiAliasing2166Hz:
                case IcmAntiAliasing2235Hz:
                case IcmAntiAliasing2306Hz:
                case IcmAntiAliasing2377Hz:
                case IcmAntiAliasing2449Hz:
                case IcmAntiAliasing2522Hz:
                case IcmAntiAliasing2596Hz:
                case IcmAntiAliasing2671Hz:
                case IcmAntiAliasing2746Hz:
                case IcmAntiAliasing2823Hz:
                case IcmAntiAliasing2900Hz:
                case IcmAntiAliasing2978Hz:
                case IcmAntiAliasing3057Hz:
                case IcmAntiAliasing3137Hz:
                case IcmAntiAliasing3217Hz:
                case IcmAntiAliasing3299Hz:
                case IcmAntiAliasing3381Hz:
                case IcmAntiAliasing3464Hz:
                case IcmAntiAliasing3548Hz:
                case IcmAntiAliasing3633Hz:
                case IcmAntiAliasing3718Hz:
                case IcmAntiAliasing3805Hz:
                case IcmAntiAliasing3892Hz:
                case IcmAntiAliasing3979Hz:
                    memcpy(metadata->value, value, metadata->size);
                    return;
            }
            break;
        case MetadataTypeIcmSampleRate:
            switch (*(IcmSampleRate*) value) {
                case IcmSampleRate32kHz:
                case IcmSampleRate16kHz:
                case IcmSampleRate8kHz:
                case IcmSampleRate4kHz:
                case IcmSampleRate2kHz:
                case IcmSampleRate1kHz:
                case IcmSampleRate500Hz:
                case IcmSampleRate200Hz:
                case IcmSampleRate100Hz:
                case IcmSampleRate50Hz:
                case IcmSampleRate25Hz:
                case IcmSampleRate12Hz:
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
    for (int index = 0; index < XIMU3_NUMBER_OF_SETTINGS; index++) {
        const Metadata metadata = MetadataGet(settings, index);
        *metadata.applied = true;
    }
}

//------------------------------------------------------------------------------
// End of file
