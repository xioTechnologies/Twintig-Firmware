// This file was generated by generate.py

#include "Ximu3Definitions.h"

Ximu3Result Ximu3SettingsIndexFrom(Ximu3SettingsIndex * const index, const int integer) {
    switch (integer) {
        case Ximu3SettingsIndexSerialNumber:
            *index = Ximu3SettingsIndexSerialNumber;
            break;
        case Ximu3SettingsIndexHardwareVersion:
            *index = Ximu3SettingsIndexHardwareVersion;
            break;
        case Ximu3SettingsIndexCalibrationDate:
            *index = Ximu3SettingsIndexCalibrationDate;
            break;
        case Ximu3SettingsIndexGyroscopeMisalignment:
            *index = Ximu3SettingsIndexGyroscopeMisalignment;
            break;
        case Ximu3SettingsIndexGyroscopeSensitivity:
            *index = Ximu3SettingsIndexGyroscopeSensitivity;
            break;
        case Ximu3SettingsIndexGyroscopeOffset:
            *index = Ximu3SettingsIndexGyroscopeOffset;
            break;
        case Ximu3SettingsIndexAccelerometerMisalignment:
            *index = Ximu3SettingsIndexAccelerometerMisalignment;
            break;
        case Ximu3SettingsIndexAccelerometerSensitivity:
            *index = Ximu3SettingsIndexAccelerometerSensitivity;
            break;
        case Ximu3SettingsIndexAccelerometerOffset:
            *index = Ximu3SettingsIndexAccelerometerOffset;
            break;
        case Ximu3SettingsIndexFirmwareVersion:
            *index = Ximu3SettingsIndexFirmwareVersion;
            break;
        case Ximu3SettingsIndexDeviceName:
            *index = Ximu3SettingsIndexDeviceName;
            break;
        case Ximu3SettingsIndexSerialEnabled:
            *index = Ximu3SettingsIndexSerialEnabled;
            break;
        case Ximu3SettingsIndexSerialBaudRate:
            *index = Ximu3SettingsIndexSerialBaudRate;
            break;
        case Ximu3SettingsIndexSerialRtsCtsEnabled:
            *index = Ximu3SettingsIndexSerialRtsCtsEnabled;
            break;
        case Ximu3SettingsIndexSampleRate:
            *index = Ximu3SettingsIndexSampleRate;
            break;
        case Ximu3SettingsIndexAxesAlignment:
            *index = Ximu3SettingsIndexAxesAlignment;
            break;
        case Ximu3SettingsIndexGyroscopeOffsetCorrectionEnabled:
            *index = Ximu3SettingsIndexGyroscopeOffsetCorrectionEnabled;
            break;
        case Ximu3SettingsIndexAhrsUpdateRateDivisor:
            *index = Ximu3SettingsIndexAhrsUpdateRateDivisor;
            break;
        case Ximu3SettingsIndexAhrsAxesConvention:
            *index = Ximu3SettingsIndexAhrsAxesConvention;
            break;
        case Ximu3SettingsIndexAhrsGain:
            *index = Ximu3SettingsIndexAhrsGain;
            break;
        case Ximu3SettingsIndexAhrsAccelerationRejection:
            *index = Ximu3SettingsIndexAhrsAccelerationRejection;
            break;
        case Ximu3SettingsIndexBinaryModeEnabled:
            *index = Ximu3SettingsIndexBinaryModeEnabled;
            break;
        case Ximu3SettingsIndexAhrsMessageType:
            *index = Ximu3SettingsIndexAhrsMessageType;
            break;
        case Ximu3SettingsIndexInertialMessageRateDivisor:
            *index = Ximu3SettingsIndexInertialMessageRateDivisor;
            break;
        case Ximu3SettingsIndexAhrsMessageRateDivisor:
            *index = Ximu3SettingsIndexAhrsMessageRateDivisor;
            break;
        case Ximu3SettingsIndexTemperatureMessageRateDivisor:
            *index = Ximu3SettingsIndexTemperatureMessageRateDivisor;
            break;
        case Ximu3SettingsIndexUsbDataMessagesEnabled:
            *index = Ximu3SettingsIndexUsbDataMessagesEnabled;
            break;
        case Ximu3SettingsIndexSerialDataMessagesEnabled:
            *index = Ximu3SettingsIndexSerialDataMessagesEnabled;
            break;
        default:
            return Ximu3ResultError;
    }
    return Ximu3ResultOk;
}
