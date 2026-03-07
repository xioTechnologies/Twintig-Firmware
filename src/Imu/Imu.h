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

extern Imu imuA;
extern Imu imuB;
extern Imu imuC;
extern Imu imuD;
extern Imu imuE;
extern Imu imuF;
extern Imu imuG;
extern Imu imuH;
extern Imu imuI;
extern Imu imuJ;
extern Imu imuK;
extern Imu imuL;
extern Imu imuM;
extern Imu imuN;
extern Imu imuO;
extern Imu imuP;
extern Imu imuQ;
extern Imu imuR;
extern Imu imuS;
extern Imu imuT;

//------------------------------------------------------------------------------
// Function declarations

void ImuTasks(Imu * const imu);
void ImuSetSettings(Imu * const imu, const ImuSettings * const settings);
void ImuReset(Imu * const imu);
void ImuSetHeading(Imu * const imu, const float heading);

#endif

//------------------------------------------------------------------------------
// End of file
