/**
 * @file Haptic.c
 * @author Seb Madgwick
 * @brief DRV2605L haptic driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "Haptic.h"
#include "I2C/I2CBB2.h"
#include "I2C/I2CStartSequence.h"
#include <stdint.h>
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief DRV2605L I2C client address.
 */
#define I2C_CLIENT_ADDRESS (0x5A)

/**
 * @brief Status register address.
 */
#define STATUS_REGISTER_ADDRESS (0x00)

/**
 * @brief Mode register address.
 */
#define MODE_REGISTER_ADDRESS (0x01)

/**
 * @brief Library selection register address.
 */
#define LIBRARY_SELECTION_REGISTER_ADDRESS (0x03)

/**
 * @brief Waveform sequencer register address.
 */
#define WAVEFORM_SEQUENCER_REGISTER_ADDRESS (0x04)

/**
 * @brief GO register address.
 */
#define GO_REGISTER_ADDRESS (0x0C)

/**
 * @brief Feedback control register address.
 */
#define FEEDBACK_CONTROL_REGISTER_ADDRESS (0x1A)

/**
 * @brief Status register.
 */
typedef union {

    struct {
        unsigned ocDetect : 1;
        unsigned overTemp : 1;
        unsigned : 1;
        unsigned diagResult : 1;
        unsigned : 1;
        unsigned deviceID : 3;
    } __attribute__((__packed__));
    uint8_t value;
} StatusRegister;

//------------------------------------------------------------------------------
// Function prototypes

static uint8_t ReadRegister(const uint8_t address);
static void WriteRegister(const uint8_t address, const uint8_t byte);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises module. This function should be called once, on system
 * start up.
 */
void HapticInitialise(void) {
    TimerDelayMicroseconds(250); // see initialise procedure on page 59 of datasheet
    WriteRegister(MODE_REGISTER_ADDRESS, 0x00); // internal trigger mode
    WriteRegister(LIBRARY_SELECTION_REGISTER_ADDRESS, 0x06); // LRA Library
    WriteRegister(FEEDBACK_CONTROL_REGISTER_ADDRESS, 0b10110110); // LRA Mode
}

/**
 * @brief Plays waveform library effect.
 * @param effect Effect ID. See page 63 of datasheet.
 * @return Result.
 */
HapticResult HapticPlay(const int effect) {
    if ((effect < 0) || (effect > 123)) {
        return HapticResultError;
    }
    WriteRegister(WAVEFORM_SEQUENCER_REGISTER_ADDRESS, (uint8_t) effect);
    WriteRegister(GO_REGISTER_ADDRESS, 0x01);
    return HapticResultOk;
}

/**
 * @brief Reads register value.
 * @param address Address.
 * @return Value.
 */
static uint8_t ReadRegister(const uint8_t address) {
    I2CBB2Start();
    I2CBB2SendAddressWrite(I2C_CLIENT_ADDRESS);
    I2CBB2Send(address);
    I2CBB2RepeatedStart();
    I2CBB2SendAddressRead(I2C_CLIENT_ADDRESS);
    const uint8_t byte = I2CBB2Receive(false);
    I2CBB2Stop();
    return byte;
}

/**
 * @brief Writes register value.
 * @param address Address.
 * @param value Value.
 */
static void WriteRegister(const uint8_t address, const uint8_t value) {
    I2CBB2Start();
    I2CBB2SendAddressWrite(I2C_CLIENT_ADDRESS);
    I2CBB2Send(address);
    I2CBB2Send(value);
    I2CBB2Stop();
}

/**
 * @brief Performs self-test.
 * @return Test result.
 */
HapticTestResult HapticTest(void) {

    // Test client ACK
    const bool ack = I2CStartSequence(&i2cBB2, I2C_CLIENT_ADDRESS, 5); // 5 ms
    I2CBB2Stop();
    if (ack == false) {
        return HapticTestResultAckFailed;
    }

    // Check device ID
    StatusRegister status = {.value = ReadRegister(STATUS_REGISTER_ADDRESS)};
    if (status.deviceID != 7) {
        return HapticTestResultInvalidId;
    }

    // Perform a diagnostic test
    WriteRegister(MODE_REGISTER_ADDRESS, 0x06); // diagnostics mode
    WriteRegister(GO_REGISTER_ADDRESS, 0x01);
    do {
        TimerDelayMilliseconds(100);
    } while (ReadRegister(GO_REGISTER_ADDRESS) == 0x01);
    WriteRegister(MODE_REGISTER_ADDRESS, 0x00); // internal trigger mode

    // Check diagnostic result
    status.value = ReadRegister(STATUS_REGISTER_ADDRESS);
    if (status.diagResult == 1) {
        return HapticTestResultDiagnosticsFailed;
    }
    if (status.overTemp == 1) {
        return HapticTestResultOverTemperature;
    }
    if (status.ocDetect == 1) {
        return HapticTestResultOverCurrent;
    }

    // Self-test passed
    return HapticTestResultPassed;
}

/**
 * @brief Returns the test result message.
 * @param result Test result.
 * @return Test result message.
 */
const char* HapticTestResultToString(const HapticTestResult result) {
    switch (result) {
        case HapticTestResultPassed:
            return "Passed";
        case HapticTestResultAckFailed:
            return "ACK failed";
        case HapticTestResultInvalidId:
            return "Invalid ID";
        case HapticTestResultDiagnosticsFailed:
            return "Diagnostic failed";
        case HapticTestResultOverTemperature:
            return "Over temperature";
        case HapticTestResultOverCurrent:
            return "Over current";
    }
    return ""; // avoid compiler warning
}

//------------------------------------------------------------------------------
// End of file
