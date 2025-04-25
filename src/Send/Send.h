/**
 * @file Send.h
 * @author Seb Madgwick
 * @brief Message sending.
 */

#ifndef SEND_H
#define SEND_H

//------------------------------------------------------------------------------
// Includes

#include "Imu/Imu.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

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
 * @brief Send settings.
 */
typedef struct {
    bool binaryModeEnabled;
    SendAhrsMessageType ahrsMessageType;
    uint32_t inertialMessageRateDivisor;
    uint32_t ahrsMessageRateDivisor;
    uint32_t temperatureMessageRateDivisor;
    bool usbDataMessagesEnabled;
    bool serialDataMessagesEnabled;
} SendSettings;

/**
 * @brief Send structure.
 */
typedef struct {
    Imu * const imu;
    SendSettings settings; // private
    FusionAhrsFlags flags; // private
    FusionVector downsampledGyroscope; // private
    FusionVector downsampledAccelerometer; // private
    uint32_t downsampledInertialCount; // private
    uint32_t downsampledAhrsCount; // private
    float downsampledTemperature; // private
    uint32_t downsampledTemperatureCount; // private
} Send;

//------------------------------------------------------------------------------
// Variable declarations

extern Send send1;
extern Send send2;
extern Send send3;
extern Send send4;
extern Send send5;
extern Send send6;
extern Send send7;
extern Send send8;
extern Send send9;
extern Send send10;
extern Send send11;
extern Send send12;
extern Send send13;
extern Send send14;
extern Send send15;
extern Send send16;
extern Send send17;
extern Send send18;
extern Send send19;
extern Send send20;

//------------------------------------------------------------------------------
// Function declarations

void SendSetSettings(Send * const send, const SendSettings * const settings);
void SendNotification(const Send * const send, const char* format, ...);
void SendError(const Send * const send, const char* format, ...);
void SendResponseUsb(const Send * const send, const void* const data, const size_t numberOfBytes);
void SendResponseSerial(const Send * const send, const void* const data, const size_t numberOfBytes);

#endif

//------------------------------------------------------------------------------
// End of file
