/**
 * @file Send.c
 * @author Seb Madgwick
 * @brief Message sending.
 */

//------------------------------------------------------------------------------
// Includes

#include "Send.h"
#include "Serial/Serial.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "Timer/Timer.h"
#include "Usb/UsbCdc.h"
#include "Ximu3Device/x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Function declarations

static void InertialDataReady(const ImuInertialData * const data, void* const context);
static void AhrsDataReady(const ImuAhrsData * const data, void* const context);
static void SendAhrsStatus(Send * const send, const uint64_t timestamp, const FusionAhrsFlags * const flags);
static void SendQuaternion(Send * const send, const ImuAhrsData * const imuData);
static void SendRotationMatrix(Send * const send, const ImuAhrsData * const imuData);
static void SendEulerAngles(Send * const send, const ImuAhrsData * const imuData);
static void SendLinearAcceleration(Send * const send, const ImuAhrsData * const imuData);
static void SendEarthAcceleration(Send * const send, const ImuAhrsData * const imuData);
static void TemperatureDataReady(const ImuTemperatureData * const data, void* const context);
static void SendDataMessage(Send * const send, const void* const data, const size_t numberOfBytes);
static void SendDataMessagePriority(Send * const send, const void* const data, const size_t numberOfBytes);
static inline __attribute__((always_inline)) size_t Write(const bool enabled,
                                                          const size_t(*getWriteAvailable)(void),
                                                          const FifoResult(*write)(const void* const data, const size_t numberOfBytes),
                                                          const void* const data, const size_t numberOfBytes,
                                                          const bool priority);

//------------------------------------------------------------------------------
// Variables

Send send1 = {.imu = &imu1};
Send send2 = {.imu = &imu1};
Send send3 = {.imu = &imu1};
Send send4 = {.imu = &imu1};
Send send5 = {.imu = &imu1};
Send send6 = {.imu = &imu1};
Send send7 = {.imu = &imu1};
Send send8 = {.imu = &imu1};
Send send9 = {.imu = &imu1};
Send send10 = {.imu = &imu1};
Send send11 = {.imu = &imu1};
Send send12 = {.imu = &imu1};
Send send13 = {.imu = &imu1};
Send send14 = {.imu = &imu1};
Send send15 = {.imu = &imu1};
Send send16 = {.imu = &imu1};
Send send17 = {.imu = &imu1};
Send send18 = {.imu = &imu1};
Send send19 = {.imu = &imu1};
Send send20 = {.imu = &imu1};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Sets the settings.
 * @param send Send structure.
 * @param settings Settings.
 */
void SendSetSettings(Send * const send, const SendSettings * const settings) {

    // Update settings
    send->settings = *settings;

    // Set callbacks
    if (send->settings.inertialMessageRateDivisor > 0) {
        send->imu->inertialDataReady = InertialDataReady;
    } else {
        send->imu->inertialDataReady = NULL;
        send->downsampledInertialCount = 0;
    }
    if (send->settings.ahrsMessageRateDivisor > 0) {
        send->imu->ahrsDataReady = AhrsDataReady;
    } else {
        send->imu->ahrsDataReady = NULL;
        send->downsampledAhrsCount = 0;
    }
    if (send->settings.temperatureMessageRateDivisor > 0) {
        send->imu->temperatureDataReady = TemperatureDataReady;
    } else {
        send->imu->temperatureDataReady = NULL;
        send->downsampledTemperatureCount = 0;
    }
    send->imu->context = send;
}

/**
 * @brief Inertial data ready callback.
 * @param data Data.
 * @param context Context.
 */
static void InertialDataReady(const ImuInertialData * const imuData, void* const context) {

    // Downsampling
    Send * const send = context;
    FusionVector gyroscope = imuData->gyroscope;
    FusionVector accelerometer = imuData->accelerometer;
    if (send->settings.inertialMessageRateDivisor > 1) {
        if (send->downsampledInertialCount == 0) {
            send->downsampledGyroscope = FUSION_VECTOR_ZERO;
            send->downsampledAccelerometer = FUSION_VECTOR_ZERO;
        }
        send->downsampledGyroscope = FusionVectorAdd(send->downsampledGyroscope, gyroscope);
        send->downsampledAccelerometer = FusionVectorAdd(send->downsampledAccelerometer, accelerometer);
        if (++send->downsampledInertialCount < send->settings.inertialMessageRateDivisor) {
            return;
        }
        const float reciprocal = 1.0f / (float) send->downsampledInertialCount;
        gyroscope = FusionVectorMultiplyScalar(send->downsampledGyroscope, reciprocal);
        accelerometer = FusionVectorMultiplyScalar(send->downsampledAccelerometer, reciprocal);
    }
    send->downsampledInertialCount = 0;

    // Send message
    const Ximu3DataInertial ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .gyroscopeX = gyroscope.axis.x,
        .gyroscopeY = gyroscope.axis.y,
        .gyroscopeZ = gyroscope.axis.z,
        .accelerometerX = accelerometer.axis.x,
        .accelerometerY = accelerometer.axis.y,
        .accelerometerZ = accelerometer.axis.z,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataInertialBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataInertialAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief AHRS data ready callback.
 * @param data Data.
 * @param context Context.
 */
static void AhrsDataReady(const ImuAhrsData * const imuData, void* const context) {

    // AHRS flags
    Send * const send = context;
    const FusionAhrsFlags flags = FusionAhrsGetFlags(imuData->ahrs);
    if (memcmp(&flags, &send->flags, sizeof (flags)) != 0) {
        send->flags = flags;
        SendAhrsStatus(send, imuData->timestamp, &flags);
    }

    // AHRS outputs
    if ((send->settings.ahrsMessageRateDivisor > 1) && (++send->downsampledAhrsCount < send->settings.ahrsMessageRateDivisor)) {
        return;
    }
    send->downsampledAhrsCount = 0;
    switch (send->settings.ahrsMessageType) {
        case SendAhrsMessageTypeQuaternion:
            SendQuaternion(send, imuData);
            break;
        case SendAhrsMessageTypeRotationMatrix:
            SendRotationMatrix(send, imuData);
            break;
        case SendAhrsMessageTypeEulerAngles:
            SendEulerAngles(send, imuData);
            break;
        case SendAhrsMessageTypeLinearAcceleration:
            SendLinearAcceleration(send, imuData);
            break;
        case SendAhrsMessageTypeEarthAcceleration:
            SendEarthAcceleration(send, imuData);
            break;
    }
}

/**
 * @brief Sends AHRS status message.
 * @param send Send structure.
 * @param timestamp Timestamp.
 * @param flags Flags.
 */
static void SendAhrsStatus(Send * const send, const uint64_t timestamp, const FusionAhrsFlags * const flags) {
    const Ximu3DataAhrsStatus ximu3Data = {
        .timestamp = timestamp / TIMER_TICKS_PER_MICROSECOND,
        .initialising = flags->initialising,
        .angularRateRecovery = flags->angularRateRecovery,
        .accelerationRecovery = flags->accelerationRecovery,
        .magneticRecovery = flags->magneticRecovery,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataAhrsStatusBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataAhrsStatusAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Sends quaternion message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendQuaternion(Send * const send, const ImuAhrsData * const imuData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(imuData->ahrs);
    const Ximu3DataQuaternion ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .w = quaternion.element.w,
        .x = quaternion.element.x,
        .y = quaternion.element.y,
        .z = quaternion. element.z,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataQuaternionBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataQuaternionAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Sends rotation matrix message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendRotationMatrix(Send * const send, const ImuAhrsData * const imuData) {
    const FusionMatrix matrix = FusionQuaternionToMatrix(FusionAhrsGetQuaternion(imuData->ahrs));
    const Ximu3DataRotationMatrix ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .xx = matrix.element.xx,
        .xy = matrix.element.xy,
        .xz = matrix.element.xz,
        .yx = matrix.element.yx,
        .yy = matrix.element.yy,
        .yz = matrix.element.yz,
        .zx = matrix.element.zx,
        .zy = matrix.element.zy,
        .zz = matrix.element.zz,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataRotationMatrixBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataRotationMatrixAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Sends Euler angles message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendEulerAngles(Send * const send, const ImuAhrsData * const imuData) {
    const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(imuData->ahrs));
    const Ximu3DataEulerAngles ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .roll = euler.angle.roll,
        .pitch = euler.angle.pitch,
        .yaw = euler.angle.yaw,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataEulerAnglesBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataEulerAnglesAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Sends linear acceleration message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendLinearAcceleration(Send * const send, const ImuAhrsData * const imuData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(imuData->ahrs);
    const FusionVector linearAcceleration = FusionAhrsGetLinearAcceleration(imuData->ahrs);
    const Ximu3DataLinearAcceleration ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .quaternionW = quaternion.element.w,
        .quaternionX = quaternion.element.x,
        .quaternionY = quaternion.element.y,
        .quaternionZ = quaternion.element.z,
        .linearAccelerationX = linearAcceleration.axis.x,
        .linearAccelerationY = linearAcceleration.axis.y,
        .linearAccelerationZ = linearAcceleration.axis.z,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataLinearAccelerationBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataLinearAccelerationAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Sends earth acceleration message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendEarthAcceleration(Send * const send, const ImuAhrsData * const imuData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(imuData->ahrs);
    const FusionVector earthAcceleration = FusionAhrsGetEarthAcceleration(imuData->ahrs);
    const Ximu3DataEarthAcceleration ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .quaternionW = quaternion.element.w,
        .quaternionX = quaternion.element.x,
        .quaternionY = quaternion.element.y,
        .quaternionZ = quaternion.element.z,
        .earthAccelerationX = earthAcceleration.axis.x,
        .earthAccelerationY = earthAcceleration.axis.y,
        .earthAccelerationZ = earthAcceleration.axis.z,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataEarthAccelerationBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataEarthAccelerationAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Temperature data ready callback.
 * @param data Data.
 * @param context Context.
 */
static void TemperatureDataReady(const ImuTemperatureData * const imuData, void* const context) {

    // Downsampling
    Send * const send = context;
    float temperature = imuData->temperature;
    if (send->settings.temperatureMessageRateDivisor > 1) {
        if (send->downsampledTemperatureCount == 0) {
            send->downsampledTemperature = 0.0f;
        }
        send->downsampledTemperature += temperature;
        if (++send->downsampledTemperatureCount < send->settings.temperatureMessageRateDivisor) {
            return;
        }
        temperature = send->downsampledTemperature / (float) send->downsampledTemperatureCount;
    }
    send->downsampledTemperatureCount = 0;

    // Send message
    const Ximu3DataTemperature ximu3Data = {
        .timestamp = imuData->timestamp / TIMER_TICKS_PER_MICROSECOND,
        .temperature = temperature,
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataTemperatureBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataTemperatureAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize);
}

/**
 * @brief Sends notification message.
 * @param send Send structure.
 * @param format Format.
 * @param ... Arguments.
 */
void SendNotification(Send * const send, const char* format, ...) {

    // Create string
    char string[256];
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(string, sizeof (string), format, arguments);
    va_end(arguments);

    // Send message
    const Ximu3DataNotification ximu3Data = {
        .timestamp = TimerGetTicks64() / TIMER_TICKS_PER_MICROSECOND,
        .string = string
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataNotificationBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataNotificationAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessagePriority(send, message, messageSize);
}

/**
 * @brief Sends error message.
 * @param send Send structure.
 * @param format Format.
 * @param ... Arguments.
 */
void SendError(Send * const send, const char* format, ...) {

    // Create string
    char string[256];
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(string, sizeof (string), format, arguments);
    va_end(arguments);

    // Send message
    const Ximu3DataError ximu3Data = {
        .timestamp = TimerGetTicks64() / TIMER_TICKS_PER_MICROSECOND,
        .string = string
    };
    uint8_t message[128];
    size_t messageSize;
    if (send->settings.binaryModeEnabled) {
        messageSize = Ximu3DataErrorBinary(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3DataErrorAscii(message, sizeof (message), &ximu3Data);
    }
    SendDataMessagePriority(send, message, messageSize);

    // Blink LED
    // TODO: blink LED
}

/**
 * @brief Sends data message.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
static void SendDataMessage(Send * const send, const void* const data, const size_t numberOfBytes) {
    if (send->settings.usbDataMessagesEnabled) {
        send->usbBufferOverflow += Write(UsbCdcPortOpen(), UsbCdcGetWriteAvailable, UsbCdcWrite, data, numberOfBytes, false);
    }
    if (send->settings.serialDataMessagesEnabled) {
        send->serialBufferOverflow += Write(SerialEnabled(), SerialGetWriteAvailable, SerialWrite, data, numberOfBytes, false);
    }
}

/**
 * @brief Sends data message with priority.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
static void SendDataMessagePriority(Send * const send, const void* const data, const size_t numberOfBytes) {
    if (send->settings.usbDataMessagesEnabled) {
        send->usbBufferOverflow += Write(UsbCdcPortOpen(), UsbCdcGetWriteAvailable, UsbCdcWrite, data, numberOfBytes, true);
    }
    if (send->settings.serialDataMessagesEnabled) {
        send->serialBufferOverflow += Write(SerialEnabled(), SerialGetWriteAvailable, SerialWrite, data, numberOfBytes, true);
    }
}

/**
 * @brief Sends response to USB.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
void SendResponseUsb(Send * const send, const void* const data, const size_t numberOfBytes) {
    send->usbBufferOverflow += Write(UsbCdcPortOpen(), UsbCdcGetWriteAvailable, UsbCdcWrite, data, numberOfBytes, true);
}

/**
 * @brief Sends response to Serial.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
void SendResponseSerial(Send * const send, const void* const data, const size_t numberOfBytes) {
    send->serialBufferOverflow += Write(SerialEnabled(), SerialGetWriteAvailable, SerialWrite, data, numberOfBytes, true);
}

/**
 * @brief Writes data and returns number of bytes lost due to buffer overflow.
 * @param enabled True if the interface is enabled.
 * @param getWriteAvailable Get write available callback function.
 * @param write Write callback function.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param priority True to write with priority.
 * @return Number of samples lost due to buffer overflow.
 */
static inline __attribute__((always_inline)) size_t Write(const bool enabled,
                                                          const size_t(*getWriteAvailable)(void),
                                                          const FifoResult(*write)(const void* const data, const size_t numberOfBytes),
                                                          const void* const data, const size_t numberOfBytes,
                                                          const bool priority) {
    if (enabled == false) {
        return 0;
    }
    if ((priority == false) && (getWriteAvailable() < (numberOfBytes + 1024))) {
        return numberOfBytes;
    }
    if (write(data, numberOfBytes) != FifoResultOK) {
        return numberOfBytes;
    }
    return 0;
}

/**
 * @brief Returns the number of bytes lost due to buffer overflow. Calling
 * this function will reset the value.
 * @return Number of samples lost due to buffer overflow.
 */
size_t SendUsbBufferOverflow(Send * const send) {
    const size_t bufferOverflow = send->usbBufferOverflow;
    send->usbBufferOverflow = 0;
    return bufferOverflow;
}

/**
 * @brief Returns the number of bytes lost due to buffer overflow. Calling
 * this function will reset the value.
 * @return Number of samples lost due to buffer overflow.
 */
size_t SendSerialBufferOverflow(Send * const send) {
    const size_t bufferOverflow = send->serialBufferOverflow;
    send->serialBufferOverflow = 0;
    return bufferOverflow;
}

//------------------------------------------------------------------------------
// End of file
