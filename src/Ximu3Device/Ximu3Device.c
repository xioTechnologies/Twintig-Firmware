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

static void InitialiseEpilogue(const bool nvmBlank, void* const context);
static void DefaultsEpilogue(const bool preservedOverwriten, void* const context);
static void WriteEpilogue(const Ximu3SettingsIndex index, void* const context);
static void Error(const char* const error, void* const context);

//------------------------------------------------------------------------------
// Variables

static const Ximu3CommandMap commands[] = {
    {"default", CommandsDefault},
    {"save", CommandsSave},
    {"ping", CommandsPing},
    {"haptic", CommandsHaptic},
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
    .nvmBlank = false,
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
 * @param nvmBlank True if NVM is blank.
 * @param context Context.
 */
static void InitialiseEpilogue(const bool nvmBlank, void* const context) {
    Context * const context_ = context;
    context_->nvmBlank = nvmBlank;
}

/**
 * @brief Defaults epilogue.
 * @param preservedOverwriten True if preserved settings overwritten.
 * @param context Context.
 */
static void DefaultsEpilogue(const bool preservedOverwriten, void* const context) {
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
