/**
 * @file Send.h
 * @author Seb Madgwick
 * @brief Message sending.
 */

#ifndef SEND_H
#define SEND_H

//------------------------------------------------------------------------------
// Includes

#include "Imu/Fusion/Fusion.h"
#include "Led/Led.h"
#include "Mux/Mux.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Data message mode.
 */
typedef enum {
    SendDataMessageModeBinary,
    SendDataMessageModeAscii,
} SendDataMessageMode;

/**
 * @brief AHRS message type.
 */
typedef enum {
    SendAhrsMessageTypeQuaternion,
    SendAhrsMessageTypeRotationMatrix,
    SendAhrsMessageTypeEulerAngles,
    SendAhrsMessageTypeLinearAcceleration,
    SendAhrsMessageTypeEarthAcceleration,
} SendAhrsMessageType;

/**
 * @brief Interface mode.
 */
typedef enum {
    SendInterfaceModeDisabled,
    SendInterfaceModeBlocking,
    SendInterfaceModeNonBlocking,
} SendInterfaceMode;

/**
 * @brief Settings.
 */
typedef struct {
    SendDataMessageMode dataMessageMode;
    SendAhrsMessageType ahrsMessageType;
    uint32_t inertialMessageRateDivisor;
    uint32_t ahrsMessageRateDivisor;
    uint32_t temperatureMessageRateDivisor;
    SendInterfaceMode usbSendMode;
    SendInterfaceMode serialSendMode;
} SendSettings;

/**
 * @brief Send structure.
 */
typedef struct {
    MuxChannel channel;
    SendSettings settings; // private
    FusionVector downsampledGyroscope; // private
    FusionVector downsampledAccelerometer; // private
    uint32_t downsampledInertialCount; // private
    FusionAhrsFlags flags; // private
    uint32_t downsampledAhrsCount; // private
    float downsampledTemperature; // private
    uint32_t downsampledTemperatureCount; // private
    size_t usbBufferOverflow; // private
    size_t serialBufferOverflow; // private
    Led * const led; // private
} Send;

/**
 * @brief Inertial data.
 */
typedef struct {
    uint64_t ticks;
    FusionVector gyroscope;
    FusionVector accelerometer;
} SendInertialData;

/**
 * @brief AHRS data.
 */
typedef struct {
    uint64_t ticks;
    const FusionAhrs * const ahrs;
} SendAhrsData;

/**
 * @brief Temperature data.
 */
typedef struct {
    uint64_t ticks;
    float temperature;
} SendTemperatureData;

//------------------------------------------------------------------------------
// Variable declarations

extern Send sendMain;
extern Send sendA;
extern Send sendB;
extern Send sendC;
extern Send sendD;
extern Send sendE;
extern Send sendF;
extern Send sendG;
extern Send sendH;
extern Send sendI;
extern Send sendJ;
extern Send sendK;
extern Send sendL;
extern Send sendM;
extern Send sendN;
extern Send sendO;
extern Send sendP;
extern Send sendQ;
extern Send sendR;
extern Send sendS;
extern Send sendT;

//------------------------------------------------------------------------------
// Function declarations

void SendSetSettings(Send * const send, const SendSettings * const settings);
void SendInertial(Send * const send, const SendInertialData * const inertialData);
void SendAhrs(Send * const send, const SendAhrsData * const ahrsData);
void SendTemperature(Send * const send, const SendTemperatureData * const temperatureData);
void SendNotification(Send * const send, const char* const format, ...);
void SendError(Send * const send, const char* const format, ...);
void SendResponseUsb(Send * const send, const void* const data, const size_t numberOfBytes);
void SendResponseSerial(Send * const send, const void* const data, const size_t numberOfBytes);
bool SendAvailable(Send * const send, const size_t numberOfBytes);
const char* SendWhoseBlocking(void);
size_t SendUsbBufferOverflow(Send * const send);
size_t SendSerialBufferOverflow(Send * const send);

#endif

//------------------------------------------------------------------------------
// End of file
