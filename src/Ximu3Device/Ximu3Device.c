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

static Ximu3CommandInterface interfaces[] = {
    { .name = "USB", .read = InterfacesUsbRead, .write = InterfacesUsbWrite},
    { .name = "Serial", .read = InterfacesSerialRead, .write = InterfacesSerialWrite},
};

static const int numberOfInterfaces = sizeof (interfaces) / sizeof (Ximu3CommandInterface);

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

static const int numberOfCommands = sizeof (commands) / sizeof (Ximu3CommandMap);

static Ximu3Settings settingsArray[] = {
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    //    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    //    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    //    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    //    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    //    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
};

static Ximu3CommandBridge bridges[] = {
    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .settings = &settingsArray[0], .numberOfCommands = numberOfCommands, .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .settings = &settingsArray[1], .numberOfCommands = numberOfCommands, .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .settings = &settingsArray[2], .numberOfCommands = numberOfCommands, .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .settings = &settingsArray[3], .numberOfCommands = numberOfCommands, .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .settings = &settingsArray[4], .numberOfCommands = numberOfCommands, .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .settings = &settingsArray[5], .numberOfCommands = numberOfCommands, .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
};

static Context contexts[] = {
    {.defaultName = "Twintig Carpus", .settings = &settingsArray[0], .nvm = &nvm0, .imu = &imu1, .send = &send0},
    //    {.defaultName = "Twintig CH1 IMU4", .settings = &settingsArray[1], .nvm = &nvm1, .imu = &imu1, .send = &send1},
    //    {.defaultName = "Twintig CH2 IMU4", .settings = &settingsArray[2], .nvm = &nvm2, .imu = &imu2, .send = &send2},
    //    {.defaultName = "Twintig CH3 IMU4", .settings = &settingsArray[3], .nvm = &nvm3, .imu = &imu3, .send = &send3},
    //    {.defaultName = "Twintig CH4 IMU4", .settings = &settingsArray[4], .nvm = &nvm4, .imu = &imu4, .send = &send4},
    //    {.defaultName = "Twintig CH5 IMU4", .settings = &settingsArray[5], .nvm = &nvm5, .imu = &imu5, .send = &send5},
};

static const int numberOfDevices = sizeof (bridges) / sizeof (Ximu3CommandBridge);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module. This function must only be called once, on
 * system startup.
 */
void Ximu3DeviceInitialise(void) {
    for (int index = 0; index < numberOfDevices; index++) {

        // Set context
        settingsArray[index].context = &contexts[index];
        bridges[index].context = &contexts[index];

        // Initialise settings
        Ximu3SettingsInitialise(&settingsArray[index]);
        ApplyNow(&contexts[index]);
    }
}

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 */
void Ximu3DeviceTasks(void) {
    for (int index = 0; index < numberOfDevices; index++) {
        Ximu3CommandTasks(&bridges[index]);
        ApplyTasks(&contexts[index]);
    }
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
    Ximu3SettingsSet(context_->settings, Ximu3SettingsIndexDeviceName, context_->defaultName, true);
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
