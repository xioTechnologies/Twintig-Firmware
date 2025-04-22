/**
 * @file Ximu3SettingsJson.c
 * @author Seb Madgwick
 * @brief x-IMU3 settings JSON.
 */

//------------------------------------------------------------------------------
// Includes

#include "KeyCompare.h"
#include "Metadata.h"
#include <stdio.h>
#include <string.h>
#include "Ximu3Settings.h"
#include "Ximu3SettingsJson.h"

//------------------------------------------------------------------------------
// Function declarations

static void Append(char* const destination, const size_t destinationSize, const char* const string);
static JsonError ParseBool(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);
static JsonError ParseCharArray(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);
static JsonError ParseFloat(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);
static JsonError ParseFusionMatrix(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);
static JsonError ParseFusionVector(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);
static JsonError ParseFloatArray(float* const destination, const char* * const value);
static JsonError ParseInt32(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);
static JsonError ParseUint32(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Gets the index.
 * @param settings Settings.
 * @param index_ Index.
 * @param key Key.
 * @return 0 if successful.
 */
int Ximu3SettingsJsonGetIndex(Ximu3Settings * const settings, Ximu3SettingsIndex * const index_, const char* const key) {
    for (int index = 0; index < XIMU3_NUMBER_OF_SETTINGS; index++) {
        if (KeyCompare(key, MetadataGet(settings, index).key)) {
            *index_ = index;
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Gets the key.
 * @param settings Settings.
 * @param destination Destination.
 * @param destinationSize Destination size.
 * @param index Index.
 */
void Ximu3SettingsJsonGetKey(Ximu3Settings * const settings, char* const destination, const size_t destinationSize, const Ximu3SettingsIndex index) {
    snprintf(destination, destinationSize, "%s", MetadataGet(settings, index).key);
}

/**
 * @brief Gets the value.
 * @param settings Settings.
 * @param destination Destination.
 * @param destinationSize Destination size.
 * @param index Index.
 */
void Ximu3SettingsJsonGetValue(Ximu3Settings * const settings, char* const destination, const size_t destinationSize, const Ximu3SettingsIndex index) {

    // Get metadata
    const Metadata metadata = MetadataGet(settings, index);

    // Write value
    switch (metadata.type) {
        case MetadataTypeBool:
            snprintf(destination, destinationSize, "%s", *(bool*) metadata.value ? "true" : "false");
            break;
        case MetadataTypeCharArray:
            snprintf(destination, destinationSize, "\"%s\"", (char*) metadata.value);
            break;
        case MetadataTypeFloat:
            snprintf(destination, destinationSize, "%f", *(float*) metadata.value);
            break;
        case MetadataTypeFusionAxesAlignment:
        case MetadataTypeFusionConvention:
        case MetadataTypeIcmOdr:
        case MetadataTypeSendAhrsMessageType:
            snprintf(destination, destinationSize, "%i", *(int*) metadata.value);
            break;
        case MetadataTypeFusionMatrix:
            snprintf(destination, destinationSize, "[[%f,%f,%f],[%f,%f,%f],[%f,%f,%f]]",
                    (*(FusionMatrix*) metadata.value).element.xx,
                    (*(FusionMatrix*) metadata.value).element.xy,
                    (*(FusionMatrix*) metadata.value).element.xz,
                    (*(FusionMatrix*) metadata.value).element.yx,
                    (*(FusionMatrix*) metadata.value).element.yy,
                    (*(FusionMatrix*) metadata.value).element.yz,
                    (*(FusionMatrix*) metadata.value).element.zx,
                    (*(FusionMatrix*) metadata.value).element.zy,
                    (*(FusionMatrix*) metadata.value).element.zz);
            break;
        case MetadataTypeFusionVector:
            snprintf(destination, destinationSize, "[%f,%f,%f]",
                    (*(FusionVector*) metadata.value).axis.x,
                    (*(FusionVector*) metadata.value).axis.y,
                    (*(FusionVector*) metadata.value).axis.z);
            break;
        case MetadataTypeUint32:
            snprintf(destination, destinationSize, "%u", *(uint32_t*) metadata.value);
            break;
    }
}

/**
 * @brief Gets the object.
 * @param settings Settings.
 * @param destination Destination.
 * @param destinationSize Destination size.
 * @param index Index.
 */
void Ximu3SettingsJsonGetObject(Ximu3Settings * const settings, char* const destination, const size_t destinationSize, const Ximu3SettingsIndex index) {
    const Metadata metadata = MetadataGet(settings, index);
    char value[XIMU3_VALUE_SIZE];
    Ximu3SettingsJsonGetValue(settings, value, sizeof (value), index);
    snprintf(destination, destinationSize, "{\"%s\":%s}", metadata.key, value);
}

/**
 * @brief Gets all settings as a single object.
 * @param settings Settings.
 * @param destination Destination.
 * @param destinationSize Destination size.
 */
void Ximu3SettingsJsonGetObjectAll(Ximu3Settings * const settings, char* const destination, const size_t destinationSize) {

    // Object start
    snprintf(destination, destinationSize, "{\n");

    // Key/value pairs
    for (int index = 0; index < XIMU3_NUMBER_OF_SETTINGS; index++) {

        // Get metadata
        const Metadata metadata = MetadataGet(settings, index);

        // Indentation
        Append(destination, destinationSize, "    ");

        // Key
        char key[XIMU3_KEY_SIZE];
        snprintf(key, sizeof (key), "\"%s\"", metadata.name);

        // Value
        char value[XIMU3_VALUE_SIZE];
        Ximu3SettingsJsonGetValue(settings, value, sizeof (value), index);

        // Key/value pair
        const size_t stringLength = strlen(destination);
        snprintf(&destination[stringLength], destinationSize - stringLength, "%-*s : %s", XIMU3_MAX_KEY_LENGTH + 2, key, value); // 2 extra characters for quotation marks

        // Comma
        if (index < (XIMU3_NUMBER_OF_SETTINGS - 1)) {
            Append(destination, destinationSize, ",");
        }
        Append(destination, destinationSize, "\n");
    }

    // Object end
    Append(destination, destinationSize, "}\n");
}

/**
 * @brief Appends string.
 * @param destination Destination.
 * @param destinationSize Destination size.
 * @param string String.
 */
static void Append(char* const destination, const size_t destinationSize, const char* const string) {
    const size_t stringLength = strlen(destination);
    snprintf(&destination[stringLength], destinationSize - stringLength, "%s", string);
}

/**
 * @brief Sets the value from a key/value pair.
 * @param settings Settings.
 * @param key Key.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
JsonError Ximu3SettingsJsonSetKeyValue(Ximu3Settings * const settings, const char* const key, const char* * const value, const bool overrideReadOnly) {

    // Get index
    Ximu3SettingsIndex index;
    if (Ximu3SettingsJsonGetIndex(settings, &index, key) != 0) {
        return JsonErrorOK;
    }

    // Get metadata
    const Metadata metadata = MetadataGet(settings, index);

    // Parse value
    switch (metadata.type) {
        case MetadataTypeBool:
            return ParseBool(settings, index, value, overrideReadOnly);
        case MetadataTypeCharArray:
            return ParseCharArray(settings, index, value, overrideReadOnly);
        case MetadataTypeFloat:
            return ParseFloat(settings, index, value, overrideReadOnly);
        case MetadataTypeFusionAxesAlignment:
        case MetadataTypeFusionConvention:
        case MetadataTypeIcmOdr:
        case MetadataTypeSendAhrsMessageType:
            return ParseInt32(settings, index, value, overrideReadOnly);
        case MetadataTypeFusionMatrix:
            return ParseFusionMatrix(settings, index, value, overrideReadOnly);
        case MetadataTypeFusionVector:
            return ParseFusionVector(settings, index, value, overrideReadOnly);
        case MetadataTypeUint32:
            return ParseUint32(settings, index, value, overrideReadOnly);
    }
    return JsonErrorOK; // avoid compiler warning
}

/**
 * @brief Parse value representing a bool.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseBool(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {
    bool boolean;
    const JsonError error = JsonParseBoolean(value, &boolean);
    if (error != JsonErrorOK) {
        return error;
    }
    Ximu3SettingsSet(settings, index, &boolean, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Parse value representing a char array.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseCharArray(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {
    char string[XIMU3_VALUE_SIZE];
    const JsonError error = JsonParseString(value, string, sizeof (string), NULL);
    if (error != JsonErrorOK) {
        return error;
    }
    Ximu3SettingsSet(settings, index, string, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Parse value representing a float.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseFloat(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {
    float number;
    const JsonError error = JsonParseNumber(value, &number);
    if (error != JsonErrorOK) {
        return error;
    }
    Ximu3SettingsSet(settings, index, &number, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Parse value representing a matrix.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseFusionMatrix(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {

    // Parse array start
    JsonError error = JsonParseArrayStart(value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse first row
    FusionMatrix matrix;
    error = ParseFloatArray(matrix.array[0], value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse comma
    error = JsonParseComma(value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse second row
    error = ParseFloatArray(matrix.array[1], value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse comma
    error = JsonParseComma(value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse third row
    error = ParseFloatArray(matrix.array[2], value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse array end
    error = JsonParseArrayEnd(value);
    if (error != JsonErrorOK) {
        return error;
    }

    Ximu3SettingsSet(settings, index, &matrix, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Parse value representing a vector.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseFusionVector(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {
    FusionVector vector;
    const JsonError error = ParseFloatArray(vector.array, value);
    if (error != JsonErrorOK) {
        return error;
    }
    Ximu3SettingsSet(settings, index, &vector, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Parse value representing an array of three floats.
 * @param destination Destination.
 * @param destinationLength Destination length.
 * @param value Value.
 * @return JSON error.
 */
static JsonError ParseFloatArray(float* const destination, const char* * const value) {

    // Parse array start
    JsonError error = JsonParseArrayStart(value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse first element
    error = JsonParseNumber(value, &destination[0]);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse comma
    error = JsonParseComma(value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse second element
    error = JsonParseNumber(value, &destination[1]);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse comma
    error = JsonParseComma(value);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse third element
    error = JsonParseNumber(value, &destination[2]);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse array end
    error = JsonParseArrayEnd(value);
    if (error != JsonErrorOK) {
        return error;
    }
    return JsonErrorOK;
}

/**
 * @brief Parse value representing an int32_t.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseInt32(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {
    float numberFloat;
    const JsonError error = JsonParseNumber(value, &numberFloat);
    if (error != JsonErrorOK) {
        return error;
    }
    const int32_t numberInt = (int32_t) numberFloat;
    Ximu3SettingsSet(settings, index, &numberInt, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Parse value representing a uint32_t.
 * @param settings Settings.
 * @param index Index.
 * @param value Value.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
static JsonError ParseUint32(Ximu3Settings * const settings, const Ximu3SettingsIndex index, const char* * const value, const bool overrideReadOnly) {
    float numberFloat;
    const JsonError error = JsonParseNumber(value, &numberFloat);
    if (error != JsonErrorOK) {
        return error;
    }
    const uint32_t numberUint32 = (uint32_t) numberFloat;
    Ximu3SettingsSet(settings, index, &numberUint32, overrideReadOnly);
    return JsonErrorOK;
}

/**
 * @brief Sets the values from an object.
 * @param settings Settings.
 * @param object_ Object.
 * @param overrideReadOnly True to override read-only.
 * @return JSON error.
 */
JsonError Ximu3SettingsJsonSetObject(Ximu3Settings * const settings, const char* object_, const bool overrideReadOnly) {

    // Parse object start
    const char* * const object = &object_;
    JsonError error = JsonParseObjectStart(object);
    if (error != JsonErrorOK) {
        return error;
    }

    // Parse object end
    error = JsonParseObjectEnd(object);
    if (error == JsonErrorOK) {
        return JsonErrorOK;
    }

    // Loop through each key/value pair
    while (true) {

        // Parse key
        char key[XIMU3_KEY_SIZE];
        error = JsonParseKey(object, key, sizeof (key));
        if (error != JsonErrorOK) {
            return error;
        }

        // Parse value
        error = Ximu3SettingsJsonSetKeyValue(settings, key, object, overrideReadOnly);
        if (error != JsonErrorOK) {
            return error;
        }

        // Parse comma
        error = JsonParseComma(object);
        if (error == JsonErrorOK) {
            continue;
        }

        // Parse object end
        error = JsonParseObjectEnd(object);
        if (error != JsonErrorOK) {
            return error;
        }
        break;
    }
    return JsonErrorOK;
}

//------------------------------------------------------------------------------
// End of file
