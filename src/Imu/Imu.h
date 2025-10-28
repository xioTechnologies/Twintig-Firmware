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
 * @brief Anti-aliasing.
 */
typedef enum {
    ImuAntiAliasingDisabled = 0,
    ImuAntiAliasing42Hz = 42,
    ImuAntiAliasing84Hz = 84,
    ImuAntiAliasing126Hz = 126,
    ImuAntiAliasing170Hz = 170,
    ImuAntiAliasing213Hz = 213,
    ImuAntiAliasing258Hz = 258,
    ImuAntiAliasing303Hz = 303,
    ImuAntiAliasing348Hz = 348,
    ImuAntiAliasing394Hz = 394,
    ImuAntiAliasing441Hz = 441,
    ImuAntiAliasing488Hz = 488,
    ImuAntiAliasing536Hz = 536,
    ImuAntiAliasing585Hz = 585,
    ImuAntiAliasing634Hz = 634,
    ImuAntiAliasing684Hz = 684,
    ImuAntiAliasing734Hz = 734,
    ImuAntiAliasing785Hz = 785,
    ImuAntiAliasing837Hz = 837,
    ImuAntiAliasing890Hz = 890,
    ImuAntiAliasing943Hz = 943,
    ImuAntiAliasing997Hz = 997,
    ImuAntiAliasing1051Hz = 1051,
    ImuAntiAliasing1107Hz = 1107,
    ImuAntiAliasing1163Hz = 1163,
    ImuAntiAliasing1220Hz = 1220,
    ImuAntiAliasing1277Hz = 1277,
    ImuAntiAliasing1336Hz = 1336,
    ImuAntiAliasing1395Hz = 1395,
    ImuAntiAliasing1454Hz = 1454,
    ImuAntiAliasing1515Hz = 1515,
    ImuAntiAliasing1577Hz = 1577,
    ImuAntiAliasing1639Hz = 1639,
    ImuAntiAliasing1702Hz = 1702,
    ImuAntiAliasing1766Hz = 1766,
    ImuAntiAliasing1830Hz = 1830,
    ImuAntiAliasing1896Hz = 1896,
    ImuAntiAliasing1962Hz = 1962,
    ImuAntiAliasing2029Hz = 2029,
    ImuAntiAliasing2097Hz = 2097,
    ImuAntiAliasing2166Hz = 2166,
    ImuAntiAliasing2235Hz = 2235,
    ImuAntiAliasing2306Hz = 2306,
    ImuAntiAliasing2377Hz = 2377,
    ImuAntiAliasing2449Hz = 2449,
    ImuAntiAliasing2522Hz = 2522,
    ImuAntiAliasing2596Hz = 2596,
    ImuAntiAliasing2671Hz = 2671,
    ImuAntiAliasing2746Hz = 2746,
    ImuAntiAliasing2823Hz = 2823,
    ImuAntiAliasing2900Hz = 2900,
    ImuAntiAliasing2978Hz = 2978,
    ImuAntiAliasing3057Hz = 3057,
    ImuAntiAliasing3137Hz = 3137,
    ImuAntiAliasing3217Hz = 3217,
    ImuAntiAliasing3299Hz = 3299,
    ImuAntiAliasing3381Hz = 3381,
    ImuAntiAliasing3464Hz = 3464,
    ImuAntiAliasing3548Hz = 3548,
    ImuAntiAliasing3633Hz = 3633,
    ImuAntiAliasing3718Hz = 3718,
    ImuAntiAliasing3805Hz = 3805,
    ImuAntiAliasing3892Hz = 3892,
    ImuAntiAliasing3979Hz = 3979,
} ImuAntiAliasing;

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
    bool gyroscopeNotchFilterEnabled;
    ImuAntiAliasing gyroscopeAntiAliasing;
    ImuAntiAliasing accelerometerAntiAliasing;
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
 * @brief IMU structure.
 */
typedef struct {
    const Icm * const icm;
    void (*inertialDataReady)(const ImuInertialData * const data, void* const context); // NULL if unused
    void (*ahrsDataReady)(const ImuAhrsData * const data, void* const context); // NULL if unused
    void (*temperatureDataReady)(const ImuTemperatureData * const data, void* const context); // NULL if unused
    void* context;
    bool initialised; // private
    ImuSettings settings; // private
    FusionOffset offset; // private
    FusionAhrs ahrs; // private
    FusionVector downsampledGyroscope; // private
    FusionVector downsampledAccelerometer; // private
    uint32_t downsampledCount; // private
    uint64_t previousTimestamp; // private
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
