/**
 * @file Imu.h
 * @author Seb Madgwick
 * @brief IMU processing.
 */

#ifndef IMU_H
#define IMU_H

//------------------------------------------------------------------------------
// Includes

#include "Fusion/Fusion.h"
#include "Icm/Icm.h"
#include "Send/Send.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Settings.
 */
typedef struct {
    FusionMatrix gyroscopeMisalignment;
    FusionVector gyroscopeSensitivity;
    FusionVector gyroscopeOffset;
    FusionMatrix accelerometerMisalignment;
    FusionVector accelerometerSensitivity;
    FusionVector accelerometerOffset;
    float sampleRate;
    FusionRemapAlignment axesRemap;
    bool gyroscopeBiasCorrectionEnabled;
    uint32_t ahrsUpdateRateDivisor;
    FusionConvention ahrsAxesConvention;
    float ahrsGain;
    float ahrsAccelerationRejection;
} ImuSettings;

/**
 * @brief IMU structure.
 */
typedef struct {
    ImuSettings settings; // private
    const Icm * const icm; // private
    bool initialised; // private
    FusionBias bias; // private
    FusionAhrs ahrs; // private
    FusionVector downsampledGyroscope; // private
    FusionVector downsampledAccelerometer; // private
    uint32_t downsampledCount; // private
    uint64_t previousTicks; // private
    Send * const send; // private
} Imu;

//------------------------------------------------------------------------------
// Variable declarations

extern Imu imu1;
extern Imu imu2;
extern Imu imu3;
extern Imu imu4;
extern Imu imu5;
extern Imu imu6;
extern Imu imu7;
extern Imu imu8;
extern Imu imu9;
extern Imu imu10;
extern Imu imu11;
extern Imu imu12;
extern Imu imu13;
extern Imu imu14;
extern Imu imu15;
extern Imu imu16;
extern Imu imu17;
extern Imu imu18;
extern Imu imu19;
extern Imu imu20;

//------------------------------------------------------------------------------
// Function declarations

void ImuTasks(Imu * const imu);
void ImuSetSettings(Imu * const imu, const ImuSettings * const settings);
void ImuReset(Imu * const imu);
void ImuSetHeading(Imu * const imu, const float heading);

#endif

//------------------------------------------------------------------------------
// End of file
