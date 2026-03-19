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
#include "Led/Led.h"
#include "Nvm.h"
#include "Send/Send.h"
#include <string.h>
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
    {"blink", CommandsBlink},
    {"strobe", CommandsStrobe},
    {"colour", CommandsColour},
    {"color", CommandsColour},
    {"haptic", CommandsHaptic},
    {"initialise", CommandsInitialise},
    {"initialize", CommandsInitialise},
    {"heading", CommandsHeading},
    {"note", CommandsNote},
    {"timestamp", CommandsTimestamp},
    {"factory", CommandsFactory},
    {"erase", CommandsErase},
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
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[6], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[7], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[8], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[9], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[10], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[11], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[12], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[13], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[14], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[15], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[16], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[17], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[18], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[19], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
    {.commands = commands, .numberOfCommands = numberOfCommands, .settings = &settingsArray[20], .overrideReadOnly = CommandsOverrideReadOnly, .writeEpilogue = WriteEpilogue, .error = Error},
};

static Context contexts[] = {
    { .settings = &settingsArray[0], .nvm = &nvmMain, .send = &sendMain, .led = &ledMain, .isMain = true},
    { .settings = &settingsArray[1], .nvm = &nvmA, .send = &sendA, .led = &ledA, .imu = &imuA, .defaultName = "Carpus IMU"},
    { .settings = &settingsArray[2], .nvm = &nvmB, .send = &sendB, .led = &ledB, .imu = &imuB, .defaultName = "I Metacarpal IMU"},
    { .settings = &settingsArray[3], .nvm = &nvmC, .send = &sendC, .led = &ledC, .imu = &imuC, .defaultName = "I Proximal IMU"},
    { .settings = &settingsArray[4], .nvm = &nvmD, .send = &sendD, .led = &ledD, .imu = &imuD, .defaultName = "I Distal IMU"},
    { .settings = &settingsArray[5], .nvm = &nvmE, .send = &sendE, .led = &ledE, .imu = &imuE, .defaultName = "II Metacarpal IMU"},
    { .settings = &settingsArray[6], .nvm = &nvmF, .send = &sendF, .led = &ledF, .imu = &imuF, .defaultName = "II Proximal IMU"},
    { .settings = &settingsArray[7], .nvm = &nvmG, .send = &sendG, .led = &ledG, .imu = &imuG, .defaultName = "II Middle IMU"},
    { .settings = &settingsArray[8], .nvm = &nvmH, .send = &sendH, .led = &ledH, .imu = &imuH, .defaultName = "II Distal IMU"},
    { .settings = &settingsArray[9], .nvm = &nvmI, .send = &sendI, .led = &ledI, .imu = &imuI, .defaultName = "III Metacarpal IMU"},
    { .settings = &settingsArray[10], .nvm = &nvmJ, .send = &sendJ, .led = &ledJ, .imu = &imuJ, .defaultName = "III Proximal IMU"},
    { .settings = &settingsArray[11], .nvm = &nvmK, .send = &sendK, .led = &ledK, .imu = &imuK, .defaultName = "III Middle IMU"},
    { .settings = &settingsArray[12], .nvm = &nvmL, .send = &sendL, .led = &ledL, .imu = &imuL, .defaultName = "III Distal IMU"},
    { .settings = &settingsArray[13], .nvm = &nvmM, .send = &sendM, .led = &ledM, .imu = &imuM, .defaultName = "IV Metacarpal IMU"},
    { .settings = &settingsArray[14], .nvm = &nvmN, .send = &sendN, .led = &ledN, .imu = &imuN, .defaultName = "IV Proximal IMU"},
    { .settings = &settingsArray[15], .nvm = &nvmO, .send = &sendO, .led = &ledO, .imu = &imuO, .defaultName = "IV Middle IMU"},
    { .settings = &settingsArray[16], .nvm = &nvmP, .send = &sendP, .led = &ledP, .imu = &imuP, .defaultName = "IV Distal IMU"},
    { .settings = &settingsArray[17], .nvm = &nvmQ, .send = &sendQ, .led = &ledQ, .imu = &imuQ, .defaultName = "V Metacarpal IMU"},
    { .settings = &settingsArray[18], .nvm = &nvmR, .send = &sendR, .led = &ledR, .imu = &imuR, .defaultName = "V Proximal IMU"},
    { .settings = &settingsArray[19], .nvm = &nvmS, .send = &sendS, .led = &ledS, .imu = &imuS, .defaultName = "V Middle IMU"},
    { .settings = &settingsArray[20], .nvm = &nvmT, .send = &sendT, .led = &ledT, .imu = &imuT, .defaultName = "V Distal IMU"},
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

    // Load all defaults if NVM blank
    Context * const context_ = context;
    const char* const calibrationDate = Ximu3SettingsGet(context_->settings)->calibrationDate;
    if (strspn(calibrationDate, "?") == strlen(calibrationDate)) { // 0xFF will be replaced with '?' when read as a string
        Ximu3SettingsLoadDefaults(context_->settings, true);
        context_->nvmBlank = true;
        return;
    }

    // Load non-preserved defaults if firmware changed
    const char* const firmwareVersion = Ximu3SettingsGet(context_->settings)->firmwareVersion;
    if (strncmp(firmwareVersion, FIRMWARE_VERSION, sizeof (FIRMWARE_VERSION)) != 0) {
        Ximu3SettingsLoadDefaults(context_->settings, false);
    }
}

/**
 * @brief Defaults epilogue.
 * @param context Context.
 */
static void DefaultsEpilogue(void* const context) {
    Context * const context_ = context;
    Ximu3SettingsSet(context_->settings, Ximu3SettingsIndexFirmwareVersion, FIRMWARE_VERSION, true);
    if (context_->isMain == false) {
        Ximu3SettingsSet(context_->settings, Ximu3SettingsIndexModel, "Twintig IMU", true);
        Ximu3SettingsSet(context_->settings, Ximu3SettingsIndexDeviceName, context_->defaultName, true);
    }
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
    Ximu3Result result = Ximu3ResultError;
    for (int index = 0; index < numberOfDevices; index++) {
        Context * const context_ = bridges[index].context;
        if (context_->send->channel == MuxChannelNone) {
            continue;
        }
        if ((channel == '^') || (channel == MuxChannelToByte(context_->send->channel))) {
            Ximu3CommandReceive(&bridges[index], interface, message, messageSize);
            result = Ximu3ResultOk;
        }
    }
    return result;
}

/**
 * @brief Error handler.
 * @param error error.
 * @param context Context.
 */
static void Error(const char* const error, void* const context) {
    Context * const context_ = context;
    SendError(context_->send, error);
}

//------------------------------------------------------------------------------
// End of file
