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
#include "Haptic/Haptic.h"
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
static Ximu3Result Mux(const Ximu3CommandInterface * const interface, const uint8_t channel, const void* const message, const size_t messageSize);
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
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
    {.nvmRead = NvmRead, .nvmWrite = NvmWrite, .initialiseEpilogue = InitialiseEpilogue, .defaultsEpilogue = DefaultsEpilogue},
};

static Ximu3CommandBridge bridges[] = {
    {.interfaces = interfaces, .numberOfInterfaces = numberOfInterfaces, .commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[0], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .mux = Mux, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[1], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[2], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[3], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[4], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[5], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[6], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[7], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[8], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[9], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[10], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[11], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[12], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[13], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[14], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[15], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[16], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[17], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[18], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[19], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    //    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[20], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
};

static Context contexts[] = {
    {.defaultName = "Twintig Carpus", .settings = &settingsArray[0], .nvm = &nvm0, .send = &send0, .serialSetSettings = &SerialSetSettings, .hapticPlay = HapticPlay},
    {.defaultName = "Twintig CH1 IMU4", .settings = &settingsArray[1], .nvm = &nvm1, .send = &send1, .imu = &imu1},
    {.defaultName = "Twintig CH2 IMU4", .settings = &settingsArray[2], .nvm = &nvm2, .send = &send2, .imu = &imu2},
    {.defaultName = "Twintig CH3 IMU4", .settings = &settingsArray[3], .nvm = &nvm3, .send = &send3, .imu = &imu3},
    {.defaultName = "Twintig CH4 IMU4", .settings = &settingsArray[4], .nvm = &nvm4, .send = &send4, .imu = &imu4},
    {.defaultName = "Twintig CH5 IMU4", .settings = &settingsArray[5], .nvm = &nvm5, .send = &send5, .imu = &imu5},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[6], .nvm = &nvm6, .send = &send6, .imu = &imu6},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[7], .nvm = &nvm7, .send = &send7, .imu = &imu7},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[8], .nvm = &nvm8, .send = &send8, .imu = &imu8},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[9], .nvm = &nvm9, .send = &send9, .imu = &imu9},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[10], .nvm = &nvm10, .send = &send10, .imu = &imu10},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[11], .nvm = &nvm11, .send = &send11, .imu = &imu11},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[12], .nvm = &nvm12, .send = &send12, .imu = &imu12},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[13], .nvm = &nvm13, .send = &send13, .imu = &imu13},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[14], .nvm = &nvm14, .send = &send14, .imu = &imu14},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[15], .nvm = &nvm15, .send = &send15, .imu = &imu15},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[16], .nvm = &nvm16, .send = &send16, .imu = &imu16},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[17], .nvm = &nvm17, .send = &send17, .imu = &imu17},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[18], .nvm = &nvm18, .send = &send18, .imu = &imu18},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[19], .nvm = &nvm19, .send = &send19, .imu = &imu19},
    //    {.defaultName = "Twintig CH? IMU?", .settings = &settingsArray[20], .nvm = &nvm20, .send = &send20, .imu = &imu20},
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
 * @brief Mux handler.
 * @param interface Interface.
 * @param channel Channel.
 * @param message Message.
 * @param messageSize Message size.
 * @return Result.
 */
static Ximu3Result Mux(const Ximu3CommandInterface * const interface, const uint8_t channel, const void* const message, const size_t messageSize) {
    switch (channel) {
        case 'A':
            Ximu3CommandReceive(&bridges[1], interface, message, messageSize);
            return Ximu3ResultOk;
        case 'B':
            Ximu3CommandReceive(&bridges[2], interface, message, messageSize);
            return Ximu3ResultOk;
        case 'C':
            Ximu3CommandReceive(&bridges[3], interface, message, messageSize);
            return Ximu3ResultOk;
        case 'D':
            Ximu3CommandReceive(&bridges[4], interface, message, messageSize);
            return Ximu3ResultOk;
        case 'E':
            Ximu3CommandReceive(&bridges[5], interface, message, messageSize);
            return Ximu3ResultOk;
    }
    return Ximu3ResultError;
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
