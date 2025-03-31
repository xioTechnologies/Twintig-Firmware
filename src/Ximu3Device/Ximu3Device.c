/**
 * @file Ximu3Device.c
 * @author Seb Madgwick
 * @brief x-IMU3 device.
 */

//------------------------------------------------------------------------------
// Includes

#include "Commands.h"
#include "Context.h"
#include "Interfaces.h"
#include "Nvm.h"
#include "Timer/Timer.h"
#include "x-IMU3-Device/Ximu3Command.h"
#include "x-IMU3-Device/Ximu3Data.h"

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
    {"save", CommandsSave},
    {"ping", CommandsPing},
    {"factory", CommandsFactory},
};
static Ximu3CommandInterface interfaces[] = {
    { .name = "USB", .read = InterfacesUsbRead, .write = InterfacesUsbWrite},
    { .name = "Serial", .read = InterfacesSerialRead, .write = InterfacesSerialWrite},
};
static Ximu3Settings settings = {
    .nvmRead = NvmRead,
    .nvmWrite = NvmWrite,
    .initialiseEpilogue = InitialiseEpilogue,
    .defaultsEpilogue = DefaultsEpilogue,
};
static Ximu3CommandBridge bridge = {
    .interfaces = interfaces,
    .numberOfInterfaces = sizeof (interfaces) / sizeof (Ximu3CommandInterface),
    .commands = commands,
    .settings = &settings,
    .numberOfCommands = sizeof (commands) / sizeof (Ximu3CommandMap),
    .overrideReadOnly = CommandsOverrideReadOnly,
    .writeEpilogue = WriteEpilogue,
    .error = Error,
};
static Context context = {
    .settings = &settings,
    .factoryMode = false,
};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module. This function must only be called once, on
 * system startup.
 */
void Ximu3DeviceInitialise(void) {
    settings.context = &context;
    bridge.context = &context;
    Ximu3SettingsInitialise(&settings);
}

/**
 * @brief Module tasks. This function should be called repeatedly within the
 * main program loop.
 */
void Ximu3DeviceTasks(void) {
    Ximu3CommandTasks(&bridge);
}

/**
 * @brief Initialise epilogue.
 * @param context Context.
 */
static void InitialiseEpilogue(void* const context) {
}

/**
 * @brief Defaults epilogue.
 * @param context Context.
 */
static void DefaultsEpilogue(void* const context) {
}

/**
 * @brief Write epilogue.
 * @param index Index.
 * @param context Context.
 */
static void WriteEpilogue(const Ximu3SettingsIndex index, void* const context) {
    // TODO: apply settings
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
