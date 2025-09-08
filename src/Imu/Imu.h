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
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Sample rate.
 */
typedef enum {
    ImuSampleRate32kHz = 32000,
    ImuSampleRate16kHz = 16000,
    ImuSampleRate8kHz = 8000,
    ImuSampleRate4kHz = 4000,
    ImuSampleRate2kHz = 2000,
    ImuSampleRate1kHz = 1000,
    ImuSampleRate500Hz = 500,
    ImuSampleRate200Hz = 200,
    ImuSampleRate100Hz = 100,
    ImuSampleRate50Hz = 50,
    ImuSampleRate25Hz = 25,
    ImuSampleRate12Hz = 12,
} ImuSampleRate;

/**
 * @brief Settings.
 */
typedef struct {
    ImuSampleRate sampleRate;
    FusionMatrix gyroscopeMisalignment;
    FusionVector gyroscopeSensitivity;
    FusionVector gyroscopeOffset;
    FusionMatrix accelerometerMisalignment;
    FusionVector accelerometerSensitivity;
    FusionVector accelerometerOffset;
    FusionAxesAlignment axisAlignment;
    bool gyroscopeOffsetEnabled;
    uint32_t ahrsUpdateRateDivisor;
    FusionConvention ahrsAxesConvention;
    float ahrsGain;
    float ahrsAccelerationRejection;
} ImuSettings;

/**
 * @brief Inertial data.
 */
typedef struct {
    uint64_t timestamp;
    FusionVector gyroscope;
    FusionVector accelerometer;
} ImuInertialData;

/**
 * @brief AHRS data.
 */
typedef struct {
    uint64_t timestamp;
    const FusionAhrs* ahrs;
} ImuAhrsData;

/**
 * @brief Temperature data.
 */
typedef struct {
    uint64_t timestamp;
    float temperature;
} ImuTemperatureData;

/**
 * @brief IMU structure. All structure members are private.
 */
typedef struct {
    const Icm * const icm;
    void (*inertialDataReady)(const ImuInertialData * const data, void* const context); // NULL if unused
    void (*ahrsDataReady)(const ImuAhrsData * const data, void* const context); // NULL if unused
    void (*temperatureDataReady)(const ImuTemperatureData * const data, void* const context); // NULL if unused
    void* context;
    bool initialised;
    ImuSettings settings;
    FusionOffset offset;
    FusionAhrs ahrs;
    FusionVector downsampledGyroscope;
    FusionVector downsampledAccelerometer;
    uint32_t downsampledCount;
    uint64_t previousTimestamp;
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
