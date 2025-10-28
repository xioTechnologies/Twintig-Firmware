/**
 * @file Send.c
 * @author Seb Madgwick
 * @brief Message sending.
 */

//------------------------------------------------------------------------------
// Includes

#include "Fifo.h"
#include "Send.h"
#include "Serial/Serial.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "Timestamp/Timestamp.h"
#include "Usb/UsbCdc.h"
#include "Ximu3Device/x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Interface.
 */
typedef struct {
    bool(*const enabled)(void);
    size_t(*const availableWrite)(const MuxChannel channel);
    FifoResult(*const write)(const MuxChannel channel, const void* const data, const size_t numberOfBytes);
} Interface;

//------------------------------------------------------------------------------
// Function declarations

static void InertialDataReady(const ImuInertialData * const data, void* const context);
static void AhrsDataReady(const ImuAhrsData * const data, void* const context);
static void SendAhrsStatus(Send * const send, const uint64_t ticks, const FusionAhrsFlags * const flags);
static void SendQuaternion(Send * const send, const ImuAhrsData * const imuData);
static void SendRotationMatrix(Send * const send, const ImuAhrsData * const imuData);
static void SendEulerAngles(Send * const send, const ImuAhrsData * const imuData);
static void SendLinearAcceleration(Send * const send, const ImuAhrsData * const imuData);
static void SendEarthAcceleration(Send * const send, const ImuAhrsData * const imuData);
static void TemperatureDataReady(const ImuTemperatureData * const data, void* const context);
static void SendDataMessage(Send * const send, const void* const data, const size_t numberOfBytes);
static void SendDataMessagePriority(Send * const send, const void* const data, const size_t numberOfBytes);
static inline __attribute__((always_inline)) size_t Write(const MuxChannel channel, const Interface * const interface, const void* const data, const size_t numberOfBytes, const bool priority);

//------------------------------------------------------------------------------
// Variables

static const Interface usb = {.enabled = UsbCdcPortOpen, .availableWrite = MuxUsbAvailableWrite, .write = MuxUsbWrite};
static const Interface serial = {.enabled = SerialEnabled, .availableWrite = MuxSerialAvailableWrite, .write = MuxSerialWrite};

Send send0 = {.channel = MuxChannelNone, .led = &led0};
Send send1 = {.channel = MuxChannel1, .led = &led1};
Send send2 = {.channel = MuxChannel2, .led = &led2};
Send send3 = {.channel = MuxChannel3, .led = &led3};
Send send4 = {.channel = MuxChannel4, .led = &led4};
Send send5 = {.channel = MuxChannel5, .led = &led5};
Send send6 = {.channel = MuxChannel6, .led = &led6};
Send send7 = {.channel = MuxChannel7, .led = &led7};
Send send8 = {.channel = MuxChannel8, .led = &led8};
Send send9 = {.channel = MuxChannel9, .led = &led9};
Send send10 = {.channel = MuxChannel10, .led = &led10};
Send send11 = {.channel = MuxChannel11, .led = &led11};
Send send12 = {.channel = MuxChannel12, .led = &led12};
Send send13 = {.channel = MuxChannel13, .led = &led13};
Send send14 = {.channel = MuxChannel14, .led = &led14};
Send send15 = {.channel = MuxChannel15, .led = &led15};
Send send16 = {.channel = MuxChannel16, .led = &led16};
Send send17 = {.channel = MuxChannel17, .led = &led17};
Send send18 = {.channel = MuxChannel18, .led = &led18};
Send send19 = {.channel = MuxChannel19, .led = &led19};
Send send20 = {.channel = MuxChannel20, .led = &led20};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Sets the settings.
 * @param send Send structure.
 * @param settings Settings.
 * @param imu IMU structure.
 */
void SendSetSettings(Send * const send, const SendSettings * const settings, Imu * const imu) {

    // Update settings
    send->settings = *settings;

    // Set callbacks
    if (imu == NULL) {
        return;
    }
    if (send->settings.inertialMessageRateDivisor > 0) {
        imu->inertialDataReady = InertialDataReady;
    } else {
        imu->inertialDataReady = NULL;
        send->downsampledInertialCount = 0;
    }
    if (send->settings.ahrsMessageRateDivisor > 0) {
        imu->ahrsDataReady = AhrsDataReady;
    } else {
        imu->ahrsDataReady = NULL;
        send->downsampledAhrsCount = 0;
    }
    if (send->settings.temperatureMessageRateDivisor > 0) {
        imu->temperatureDataReady = TemperatureDataReady;
    } else {
        imu->temperatureDataReady = NULL;
        send->downsampledTemperatureCount = 0;
    }
    imu->context = send;
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
        .timestamp = TimestampFrom(imuData->ticks),
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
        SendAhrsStatus(send, imuData->ticks, &flags);
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
 * @brief Sends an AHRS status message.
 * @param send Send structure.
 * @param ticks Ticks.
 * @param flags Flags.
 */
static void SendAhrsStatus(Send * const send, const uint64_t ticks, const FusionAhrsFlags * const flags) {
    const Ximu3DataAhrsStatus ximu3Data = {
        .timestamp = TimestampFrom(ticks),
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
 * @brief Sends a quaternion message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendQuaternion(Send * const send, const ImuAhrsData * const imuData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(imuData->ahrs);
    const Ximu3DataQuaternion ximu3Data = {
        .timestamp = TimestampFrom(imuData->ticks),
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
 * @brief Sends a rotation matrix message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendRotationMatrix(Send * const send, const ImuAhrsData * const imuData) {
    const FusionMatrix matrix = FusionQuaternionToMatrix(FusionAhrsGetQuaternion(imuData->ahrs));
    const Ximu3DataRotationMatrix ximu3Data = {
        .timestamp = TimestampFrom(imuData->ticks),
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
 * @brief Sends an Euler angles message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendEulerAngles(Send * const send, const ImuAhrsData * const imuData) {
    const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(imuData->ahrs));
    const Ximu3DataEulerAngles ximu3Data = {
        .timestamp = TimestampFrom(imuData->ticks),
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
 * @brief Sends a linear acceleration message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendLinearAcceleration(Send * const send, const ImuAhrsData * const imuData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(imuData->ahrs);
    const FusionVector linearAcceleration = FusionAhrsGetLinearAcceleration(imuData->ahrs);
    const Ximu3DataLinearAcceleration ximu3Data = {
        .timestamp = TimestampFrom(imuData->ticks),
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
 * @brief Sends an earth acceleration message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendEarthAcceleration(Send * const send, const ImuAhrsData * const imuData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(imuData->ahrs);
    const FusionVector earthAcceleration = FusionAhrsGetEarthAcceleration(imuData->ahrs);
    const Ximu3DataEarthAcceleration ximu3Data = {
        .timestamp = TimestampFrom(imuData->ticks),
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
        .timestamp = TimestampFrom(imuData->ticks),
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
 * @brief Sends a notification message.
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
        .timestamp = TimestampGet(),
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
 * @brief Sends an error message.
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
        .timestamp = TimestampGet(),
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
    LedBlink(send->led, ledColourRed);
}

/**
 * @brief Sends a data message.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
static void SendDataMessage(Send * const send, const void* const data, const size_t numberOfBytes) {
    if (send->settings.usbDataMessagesEnabled) {
        send->usbBufferOverflow += Write(send->channel, &usb, data, numberOfBytes, false);
    }
    if (send->settings.serialDataMessagesEnabled) {
        send->serialBufferOverflow += Write(send->channel, &serial, data, numberOfBytes, false);
    }
}

/**
 * @brief Sends a data message with priority.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
static void SendDataMessagePriority(Send * const send, const void* const data, const size_t numberOfBytes) {
    if (send->settings.usbDataMessagesEnabled) {
        send->usbBufferOverflow += Write(send->channel, &usb, data, numberOfBytes, true);
    }
    if (send->settings.serialDataMessagesEnabled) {
        send->serialBufferOverflow += Write(send->channel, &serial, data, numberOfBytes, true);
    }
}

/**
 * @brief Sends a response to USB.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
void SendResponseUsb(Send * const send, const void* const data, const size_t numberOfBytes) {
    send->usbBufferOverflow += Write(send->channel, &usb, data, numberOfBytes, true);
}

/**
 * @brief Sends a response to serial.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
void SendResponseSerial(Send * const send, const void* const data, const size_t numberOfBytes) {
    send->serialBufferOverflow += Write(send->channel, &serial, data, numberOfBytes, true);
}

/**
 * @brief Writes data and returns the number of bytes lost due to buffer
 * overflow.
 * @param channel Channel.
 * @param interface Interface.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param priority True to write with priority.
 * @return Number of samples lost due to buffer overflow.
 */
static inline __attribute__((always_inline)) size_t Write(const MuxChannel channel, const Interface * const interface, const void* const data, const size_t numberOfBytes, const bool priority) {
    if (interface->enabled() == false) {
        return 0;
    }
    if ((priority == false) && (interface->availableWrite(channel) < (numberOfBytes + (20 * 1024)))) {
        return numberOfBytes;
    }
    if (interface->write(channel, data, numberOfBytes) != FifoResultOk) {
        return numberOfBytes;
    }
    return 0;
}

/**
 * @brief Returns the number of bytes lost due to buffer overflow. Calling
 * this function will reset the value.
 * @param send Send structure.
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
 * @param send Send structure.
 * @return Number of samples lost due to buffer overflow.
 */
size_t SendSerialBufferOverflow(Send * const send) {
    const size_t bufferOverflow = send->serialBufferOverflow;
    send->serialBufferOverflow = 0;
    return bufferOverflow;
}

//------------------------------------------------------------------------------
// End of file
