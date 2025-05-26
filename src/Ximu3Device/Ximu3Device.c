/**
 * @file Ximu3Device.c
 * @author Seb Madgwick
 * @brief x-IMU3 device.
 */

//------------------------------------------------------------------------------
// Includes

#include "Apply.h"
#include "Commands.h"
#include "Context.h"
#include "FirmwareVersion.h"
#include "Imu/Imu.h"
#include "Interfaces.h"
#include "Nvm.h"
#include "Send/Send.h"
#include <string.h>
#include "Timer/Timer.h"
#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Function declarations

static void InitialiseEpilogue(void* const context);
static void DefaultsEpilogue(void* const context);
static void WriteEpilogue(const Ximu3SettingsIndex index, void* const context);
static void Error(const char* const error, void* const context);

//------------------------------------------------------------------------------
// Variables

static const Ximu3CommandMap commands[] = {
    {"default", CommandsDefault},
    {"apply", CommandsApply},
    {"save", CommandsSave},
    {"ping", CommandsPing},
    {"haptic", CommandsHaptic},
    {"initialise", CommandsInitialise},
    {"initialize", CommandsInitialise},
    {"heading", CommandsHeading},
    {"note", CommandsNote},
    {"factory", CommandsFactory},
};
static Ximu3CommandInterface interfaces[] = {
    { .name = "USB", .read = InterfacesUsbRead, .write = InterfacesUsbWrite},
    { .name = "Serial", .read = InterfacesSerialRead, .write = InterfacesSerialWrite},
};
static Ximu3Settings settings1 = {
    .nvmRead = NvmRead,
    .nvmWrite = NvmWrite,
    .initialiseEpilogue = InitialiseEpilogue,
    .defaultsEpilogue = DefaultsEpilogue,
};
static Ximu3CommandBridge bridge1 = {
    .interfaces = interfaces,
    .numberOfInterfaces = sizeof (interfaces) / sizeof (Ximu3CommandInterface),
    .commands = commands,
    .settings = &settings1,
    .numberOfCommands = sizeof (commands) / sizeof (Ximu3CommandMap),
    .overrideReadOnly = CommandsOverrideReadOnly,
    .writeEpilogue = WriteEpilogue,
    .error = Error,
};
static Context context1 = {
    .settings = &settings1,
    .nvm = &nvm1,
    .imu = &imu1,
    .send = &send1,
};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module. This function must only be called once, on
 * system startup.
 */
void Ximu3DeviceInitialise(void) {
    settings1.context = &context1;
    bridge1.context = &context1;
    Ximu3SettingsInitialise(&settings1);
    ApplyNow(&context1);
}

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 */
void Ximu3DeviceTasks(void) {
    Ximu3CommandTasks(&bridge1);
    ApplyTasks(&context1);
}

/**
 * @brief Initialise epilogue.
 * @param context Context.
 */
static void InitialiseEpilogue(void* const context) {
    Context * const context_ = context;
    const char* const firmwareVersion = Ximu3SettingsGet(context_->settings)->firmwareVersion;
    if (strspn(firmwareVersion, "?") == strlen(firmwareVersion)) { // if NVM blank
        Ximu3SettingsDefaults(context_->settings, true);
        Context * const context_ = context;
        context_->nvmBlank = true;
        return;
    }
    if (strncmp(firmwareVersion, FIRMWARE_VERSION, sizeof (FIRMWARE_VERSION)) != 0) { // if firmware changed
        Ximu3SettingsDefaults(context_->settings, false);
    }
}

/**
 * @brief Defaults epilogue.
 * @param context Context.
 */
static void DefaultsEpilogue(void* const context) {
    Context * const context_ = context;
    Ximu3SettingsSet(context_->settings, Ximu3SettingsIndexFirmwareVersion, FIRMWARE_VERSION, true);
}

/**
 * @brief Write epilogue.
 * @param index Index.
 * @param context Context.
 */
static void WriteEpilogue(const Ximu3SettingsIndex index, void* const context) {
    Context * const context_ = context;
    ApplyAfterDelay(context_);
}

/**
 * @brief Error handler.
 * @param error error.
 * @param context Context.
 */
static void Error(const char* const error, void* const context) {
    const Ximu3DataError data = {
        .timestamp = TimerGetTicks64() / TIMER_TICKS_PER_MICROSECOND,
        .string = error,
    };
    char message[256];
    const int numberOfBytes = Ximu3DataErrorAscii(message, sizeof (message), &data);
    InterfacesUsbWrite(message, numberOfBytes, context);
    InterfacesSerialWrite(message, numberOfBytes, context);
}

//------------------------------------------------------------------------------
// End of file
