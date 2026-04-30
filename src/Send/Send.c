/**
 * @file Send.c
 * @author Seb Madgwick
 * @brief Message sending.
 *
 * Send priority:
 * High     Error data messages
 * Medium   Command responses and notification data messages
 * Low      All other data messages
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

/**
 * @brief Priority.
 */
typedef enum {
    PriorityLow,
    PriorityMedium,
    PriorityHigh,
} Priority;

/**
 * @brief Medium-priority buffer size.
 */
#define MEDIUM_PRIORITY_BUFFER_SIZE (2048)

/**
 * @brief High-priority buffer size.
 */
#define HIGH_PRIORITY_BUFFER_SIZE (1024)

//------------------------------------------------------------------------------
// Function declarations

static void SendAhrsStatus(Send * const send, const uint64_t ticks, const FusionAhrsFlags * const flags);
static void SendQuaternion(Send * const send, const SendAhrsData * const ahrsData);
static void SendRotationMatrix(Send * const send, const SendAhrsData * const ahrsData);
static void SendEulerAngles(Send * const send, const SendAhrsData * const ahrsData);
static void SendLinearAcceleration(Send * const send, const SendAhrsData * const ahrsData);
static void SendEarthAcceleration(Send * const send, const SendAhrsData * const ahrsData);
static void SendDataMessage(Send * const send, const void* const data, const size_t numberOfBytes, const Priority priority);
static inline __attribute__((always_inline)) size_t Write(const MuxChannel channel, const Interface * const interface, const void* const data, const size_t numberOfBytes, const Priority priority);
static inline __attribute__((always_inline)) bool AvailableWrite(const MuxChannel channel, const Interface * const interface, const size_t numberOfBytes, const Priority priority);
static inline __attribute__((always_inline)) bool Blocked(const MuxChannel channel, const SendInterfaceMode mode, const Interface * const interface, const size_t numberOfBytes);

//------------------------------------------------------------------------------
// Variables

static const Interface usb = {.enabled = UsbCdcPortOpen, .availableWrite = MuxUsbAvailableWrite, .write = MuxUsbWrite};
static const Interface serial = {.enabled = SerialEnabled, .availableWrite = MuxSerialAvailableWrite, .write = MuxSerialWrite};

static const char* whoseBlocking = "";

Send sendMain = {.channel = MuxChannelNone, .led = &ledMain};
Send sendA = {.channel = MuxChannelA, .led = &ledA};
Send sendB = {.channel = MuxChannelB, .led = &ledB};
Send sendC = {.channel = MuxChannelC, .led = &ledC};
Send sendD = {.channel = MuxChannelD, .led = &ledD};
Send sendE = {.channel = MuxChannelE, .led = &ledE};
Send sendF = {.channel = MuxChannelF, .led = &ledF};
Send sendG = {.channel = MuxChannelG, .led = &ledG};
Send sendH = {.channel = MuxChannelH, .led = &ledH};
Send sendI = {.channel = MuxChannelI, .led = &ledI};
Send sendJ = {.channel = MuxChannelJ, .led = &ledJ};
Send sendK = {.channel = MuxChannelK, .led = &ledK};
Send sendL = {.channel = MuxChannelL, .led = &ledL};
Send sendM = {.channel = MuxChannelM, .led = &ledM};
Send sendN = {.channel = MuxChannelN, .led = &ledN};
Send sendO = {.channel = MuxChannelO, .led = &ledO};
Send sendP = {.channel = MuxChannelP, .led = &ledP};
Send sendQ = {.channel = MuxChannelQ, .led = &ledQ};
Send sendR = {.channel = MuxChannelR, .led = &ledR};
Send sendS = {.channel = MuxChannelS, .led = &ledS};
Send sendT = {.channel = MuxChannelT, .led = &ledT};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Sets the settings.
 * @param send Send structure.
 * @param settings Settings.
 */
void SendSetSettings(Send * const send, const SendSettings * const settings) {
    send->settings = *settings;
}

/**
 * @brief Sends an inertial message.
 * @param send Send structure.
 * @param inertialData Inertial data.
 */
void SendInertial(Send * const send, const SendInertialData * const inertialData) {

    // Message disabled
    if (send->settings.inertialMessageRateDivisor == 0) {
        send->downsampledInertialCount = 0;
        return;
    }

    // Downsampling
    FusionVector gyroscope = inertialData->gyroscope;
    FusionVector accelerometer = inertialData->accelerometer;
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
        gyroscope = FusionVectorScale(send->downsampledGyroscope, reciprocal);
        accelerometer = FusionVectorScale(send->downsampledAccelerometer, reciprocal);
    }
    send->downsampledInertialCount = 0;

    // Send message
    const Ximu3DataInertial ximu3Data = {
        .timestamp = TimestampFrom(inertialData->ticks),
        .gyroscopeX = gyroscope.axis.x,
        .gyroscopeY = gyroscope.axis.y,
        .gyroscopeZ = gyroscope.axis.z,
        .accelerometerX = accelerometer.axis.x,
        .accelerometerY = accelerometer.axis.y,
        .accelerometerZ = accelerometer.axis.z,
    };
    uint8_t message[XIMU3_SIZE_INERTIAL];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryInertial(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiInertial(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends an AHRS message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
void SendAhrs(Send * const send, const SendAhrsData * const ahrsData) {

    // Message disabled
    if (send->settings.ahrsMessageRateDivisor == 0) {
        send->downsampledAhrsCount = 0;
        return;
    }

    // AHRS flags
    const FusionAhrsFlags flags = FusionAhrsGetFlags(ahrsData->ahrs);
    if (memcmp(&flags, &send->flags, sizeof (flags)) != 0) {
        send->flags = flags;
        SendAhrsStatus(send, ahrsData->ticks, &flags);
    }

    // AHRS outputs
    if ((send->settings.ahrsMessageRateDivisor > 1) && (++send->downsampledAhrsCount < send->settings.ahrsMessageRateDivisor)) {
        return;
    }
    send->downsampledAhrsCount = 0;
    switch (send->settings.ahrsMessageType) {
        case SendAhrsMessageTypeQuaternion:
            SendQuaternion(send, ahrsData);
            break;
        case SendAhrsMessageTypeRotationMatrix:
            SendRotationMatrix(send, ahrsData);
            break;
        case SendAhrsMessageTypeEulerAngles:
            SendEulerAngles(send, ahrsData);
            break;
        case SendAhrsMessageTypeLinearAcceleration:
            SendLinearAcceleration(send, ahrsData);
            break;
        case SendAhrsMessageTypeEarthAcceleration:
            SendEarthAcceleration(send, ahrsData);
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
        .initialising = flags->startup,
        .angularRateRecovery = flags->angularRateRecovery,
        .accelerationRecovery = flags->accelerationRecovery,
        .magneticRecovery = flags->magneticRecovery,
    };
    uint8_t message[XIMU3_SIZE_AHRS_STATUS];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryAhrsStatus(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiAhrsStatus(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends a quaternion message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendQuaternion(Send * const send, const SendAhrsData * const ahrsData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(ahrsData->ahrs);
    const Ximu3DataQuaternion ximu3Data = {
        .timestamp = TimestampFrom(ahrsData->ticks),
        .w = quaternion.element.w,
        .x = quaternion.element.x,
        .y = quaternion.element.y,
        .z = quaternion.element.z,
    };
    uint8_t message[XIMU3_SIZE_QUATERNION];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryQuaternion(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiQuaternion(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends a rotation matrix message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendRotationMatrix(Send * const send, const SendAhrsData * const ahrsData) {
    const FusionMatrix matrix = FusionQuaternionToMatrix(FusionAhrsGetQuaternion(ahrsData->ahrs));
    const Ximu3DataRotationMatrix ximu3Data = {
        .timestamp = TimestampFrom(ahrsData->ticks),
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
    uint8_t message[XIMU3_SIZE_ROTATION_MATRIX];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryRotationMatrix(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiRotationMatrix(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends an Euler angles message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendEulerAngles(Send * const send, const SendAhrsData * const ahrsData) {
    const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(ahrsData->ahrs));
    const Ximu3DataEulerAngles ximu3Data = {
        .timestamp = TimestampFrom(ahrsData->ticks),
        .roll = euler.angle.roll,
        .pitch = euler.angle.pitch,
        .yaw = euler.angle.yaw,
    };
    uint8_t message[XIMU3_SIZE_EULER_ANGLES];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryEulerAngles(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiEulerAngles(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends a linear acceleration message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendLinearAcceleration(Send * const send, const SendAhrsData * const ahrsData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(ahrsData->ahrs);
    const FusionVector linearAcceleration = FusionAhrsGetLinearAcceleration(ahrsData->ahrs);
    const Ximu3DataLinearAcceleration ximu3Data = {
        .timestamp = TimestampFrom(ahrsData->ticks),
        .quaternionW = quaternion.element.w,
        .quaternionX = quaternion.element.x,
        .quaternionY = quaternion.element.y,
        .quaternionZ = quaternion.element.z,
        .linearAccelerationX = linearAcceleration.axis.x,
        .linearAccelerationY = linearAcceleration.axis.y,
        .linearAccelerationZ = linearAcceleration.axis.z,
    };
    uint8_t message[XIMU3_SIZE_LINEAR_ACCELERATION];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryLinearAcceleration(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiLinearAcceleration(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends an earth acceleration message.
 * @param send Send structure.
 * @param ahrsData AHRS data.
 */
static void SendEarthAcceleration(Send * const send, const SendAhrsData * const ahrsData) {
    const FusionQuaternion quaternion = FusionAhrsGetQuaternion(ahrsData->ahrs);
    const FusionVector earthAcceleration = FusionAhrsGetEarthAcceleration(ahrsData->ahrs);
    const Ximu3DataEarthAcceleration ximu3Data = {
        .timestamp = TimestampFrom(ahrsData->ticks),
        .quaternionW = quaternion.element.w,
        .quaternionX = quaternion.element.x,
        .quaternionY = quaternion.element.y,
        .quaternionZ = quaternion.element.z,
        .earthAccelerationX = earthAcceleration.axis.x,
        .earthAccelerationY = earthAcceleration.axis.y,
        .earthAccelerationZ = earthAcceleration.axis.z,
    };
    uint8_t message[XIMU3_SIZE_EARTH_ACCELERATION];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryEarthAcceleration(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiEarthAcceleration(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends a temperature message.
 * @param send Send structure.
 * @param temperatureData Temperature data.
 */
void SendTemperature(Send * const send, const SendTemperatureData * const temperatureData) {

    // Message disabled
    if (send->settings.temperatureMessageRateDivisor == 0) {
        send->downsampledTemperatureCount = 0;
        return;
    }

    // Downsampling
    float temperature = temperatureData->temperature;
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
        .timestamp = TimestampFrom(temperatureData->ticks),
        .temperature = temperature,
    };
    uint8_t message[XIMU3_SIZE_TEMPERATURE];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryTemperature(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiTemperature(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityLow);
}

/**
 * @brief Sends a notification message.
 * @param send Send structure.
 * @param format Format.
 * @param ... Arguments.
 */
void SendNotification(Send * const send, const char* const format, ...) {

    // Create string
    char notification[XIMU3_SIZE_CHAR_ARRAY];
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(notification, sizeof (notification), format, arguments);
    va_end(arguments);

    // Send message
    const Ximu3DataNotification ximu3Data = {
        .timestamp = TimestampGet(),
        .notification = notification,
    };
    uint8_t message[XIMU3_SIZE_NOTIFICATION];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryNotification(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiNotification(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityMedium);
}

/**
 * @brief Sends an error message.
 * @param send Send structure.
 * @param format Format.
 * @param ... Arguments.
 */
void SendError(Send * const send, const char* const format, ...) {

    // Create string
    char error[XIMU3_SIZE_CHAR_ARRAY];
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(error, sizeof (error), format, arguments);
    va_end(arguments);

    // Send message
    const Ximu3DataError ximu3Data = {
        .timestamp = TimestampGet(),
        .error = error,
    };
    uint8_t message[XIMU3_SIZE_ERROR];
    size_t messageSize;
    if (send->settings.dataMessageMode == SendDataMessageModeBinary) {
        messageSize = Ximu3BinaryError(message, sizeof (message), &ximu3Data);
    } else {
        messageSize = Ximu3AsciiError(message, sizeof (message), &ximu3Data);
    }
    SendDataMessage(send, message, messageSize, PriorityHigh);

    // Blink LED
    LedBlink(send->led, ledColourRed);
}

/**
 * @brief Sends a data message.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param priority Priority.
 */
static void SendDataMessage(Send * const send, const void* const data, const size_t numberOfBytes, const Priority priority) {
    if (send->settings.usbSendMode != SendInterfaceModeDisabled) {
        send->usbBufferOverflow += Write(send->channel, &usb, data, numberOfBytes, priority);
    }
    if (send->settings.serialSendMode != SendInterfaceModeDisabled) {
        send->serialBufferOverflow += Write(send->channel, &serial, data, numberOfBytes, priority);
    }
}

/**
 * @brief Sends a response to USB.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
void SendResponseUsb(Send * const send, const void* const data, const size_t numberOfBytes) {
    send->usbBufferOverflow += Write(send->channel, &usb, data, numberOfBytes, PriorityMedium);
}

/**
 * @brief Sends a response to serial.
 * @param send Send structure.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 */
void SendResponseSerial(Send * const send, const void* const data, const size_t numberOfBytes) {
    send->serialBufferOverflow += Write(send->channel, &serial, data, numberOfBytes, PriorityMedium);
}

/**
 * @brief Writes data and returns the number of bytes lost due to buffer
 * overflow.
 * @param channel Channel.
 * @param interface Interface.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param priority Priority.
 * @return Number of bytes lost due to buffer overflow.
 */
static inline __attribute__((always_inline)) size_t Write(const MuxChannel channel, const Interface * const interface, const void* const data, const size_t numberOfBytes, const Priority priority) {
    if (interface->enabled() == false) {
        return 0;
    }
    if (AvailableWrite(channel, interface, numberOfBytes, priority) == false) {
        return numberOfBytes;
    }
    if (interface->write(channel, data, numberOfBytes) != FifoResultOk) {
        return numberOfBytes;
    }
    return 0;
}

/**
 * @brief Returns true if there is enough space available in the write buffer.
 * @param channel Channel.
 * @param interface Interface.
 * @param numberOfBytes Number of bytes.
 * @param priority Priority.
 * @return True if there is enough space available in the write buffer.
 */
static inline __attribute__((always_inline)) bool AvailableWrite(const MuxChannel channel, const Interface * const interface, const size_t numberOfBytes, const Priority priority) {
    switch (priority) {
        case PriorityLow:
            return interface->availableWrite(channel) > (numberOfBytes + HIGH_PRIORITY_BUFFER_SIZE + MEDIUM_PRIORITY_BUFFER_SIZE);
        case PriorityMedium:
            return interface->availableWrite(channel) > (numberOfBytes + HIGH_PRIORITY_BUFFER_SIZE);
        case PriorityHigh:
            return interface->availableWrite(channel) > numberOfBytes;
    }
    return false; // avoid compiler warning
}

/**
 * @brief Returns true if there is enough space available for low-priority data
 * messages. Only enabled interfaces in blocking mode will limit availability.
 * @param send Send structure.
 * @param numberOfBytes Number of bytes.
 * @return True if there is enough space available for low-priority data
 * messages.
 */
bool SendAvailable(Send * const send, const size_t numberOfBytes) {
    if (Blocked(send->channel, send->settings.usbSendMode, &usb, numberOfBytes)) {
        whoseBlocking = "USB";
        return false;
    }
    if (Blocked(send->channel, send->settings.serialSendMode, &serial, numberOfBytes)) {
        whoseBlocking = "Serial";
        return false;
    }
    return true;
}

/**
 * @brief Returns true if the interface is blocked.
 * @param channel Channel.
 * @param mode Mode.
 * @param interface Interface.
 * @param numberOfBytes Number of bytes.
 * @return True if the interface is blocked.
 */
static inline __attribute__((always_inline)) bool Blocked(const MuxChannel channel, const SendInterfaceMode mode, const Interface * const interface, const size_t numberOfBytes) {
    return (mode == SendInterfaceModeBlocking) && interface->enabled() && (AvailableWrite(channel, interface, numberOfBytes, PriorityLow) == false);
}

/**
 * @brief Returns the name of the last interface to block sending.
 * @return The name of the last interface to block sending.
 */
const char* SendWhoseBlocking(void) {
    return whoseBlocking;
}

/**
 * @brief Returns the number of bytes lost due to buffer overflow. Calling
 * this function will reset the value.
 * @param send Send structure.
 * @return Number of bytes lost due to buffer overflow.
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
 * @return Number of bytes lost due to buffer overflow.
 */
size_t SendSerialBufferOverflow(Send * const send) {
    const size_t bufferOverflow = send->serialBufferOverflow;
    send->serialBufferOverflow = 0;
    return bufferOverflow;
}

//------------------------------------------------------------------------------
// End of file
