// This file was generated by generate.py

#include "Metadata.h"

static const char* const names[] = {
    "Calibration Date",
    "Offset",
    "Sensitivity",
    "Device Name",
    "Serial Number",
    "Firmware Version",
    "Binary Mode",
    "Message Rate Divisor",
};

static const char* const keys[] = {
    "calibration_date",
    "offset",
    "sensitivity",
    "device_name",
    "serial_number",
    "firmware_version",
    "binary_mode",
    "message_rate_divisor",
};

const MetadataType types[] = {
    MetadataTypeCharArray,
    MetadataTypeFloat,
    MetadataTypeFloat,
    MetadataTypeCharArray,
    MetadataTypeCharArray,
    MetadataTypeCharArray,
    MetadataTypeBool,
    MetadataTypeUint32,
};

const size_t sizes[] = {
    sizeof (((Ximu3SettingsValues *) 0)->calibrationDate),
    sizeof (((Ximu3SettingsValues *) 0)->offset),
    sizeof (((Ximu3SettingsValues *) 0)->sensitivity),
    sizeof (((Ximu3SettingsValues *) 0)->deviceName),
    sizeof (((Ximu3SettingsValues *) 0)->serialNumber),
    sizeof (((Ximu3SettingsValues *) 0)->firmwareVersion),
    sizeof (((Ximu3SettingsValues *) 0)->binaryMode),
    sizeof (((Ximu3SettingsValues *) 0)->messageRateDivisor),
};

const void* const defaults[] = {
    (void*) (&(char[32]) {"Unknown"}),
    (void*) (&(float) {0.0f}),
    (void*) (&(float) {1.0f}),
    (void*) (&(char[32]) {"x-IMU3"}),
    (void*) (&(char[16]) {"Unknown"}),
    (void*) (&(char[32]) {"Unknown"}),
    (void*) (&(bool) {false}),
    (void*) (&(uint32_t) {1}),
};

const bool calibrations[] = {
    true,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
};

const bool readOnlys[] = {
    true,
    true,
    true,
    false,
    true,
    true,
    false,
    false,
};

static void* GetValue(Ximu3Settings * const settings, const Ximu3SettingsIndex index) {
    switch (index) {
        case Ximu3SettingsIndexCalibrationDate:
            return &settings->values.calibrationDate;
        case Ximu3SettingsIndexOffset:
            return &settings->values.offset;
        case Ximu3SettingsIndexSensitivity:
            return &settings->values.sensitivity;
        case Ximu3SettingsIndexDeviceName:
            return &settings->values.deviceName;
        case Ximu3SettingsIndexSerialNumber:
            return &settings->values.serialNumber;
        case Ximu3SettingsIndexFirmwareVersion:
            return &settings->values.firmwareVersion;
        case Ximu3SettingsIndexBinaryMode:
            return &settings->values.binaryMode;
        case Ximu3SettingsIndexMessageRateDivisor:
            return &settings->values.messageRateDivisor;

    }
    return NULL; // avoid compiler warning
}

Metadata MetadataGet(Ximu3Settings * const settings, const Ximu3SettingsIndex index) {
    const Metadata metaData = {
        .name = names[index],
        .key = keys[index],
        .value = GetValue(settings, index),
        .type = types[index],
        .size = sizes[index],
        .defaultValue = defaults[index],
        .calibration = calibrations[index],
        .readOnly = readOnlys[index],
        .applyPending = &settings->applyPendings[index],
    };
    return metaData;
}
