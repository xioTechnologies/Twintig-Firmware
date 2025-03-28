/**
 * @file Haptic.c
 * @author Seb Madgwick
 * @brief Driver for the DRV2605L haptic driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "Haptic.h"
#include "I2C/I2CBitBang.h"
#include "I2C/I2CClientAddress.h"
#include <stdint.h>
#include "Timer/Timer.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief DRV2605L I2C client address.
 */
#define I2C_CLIENT_ADDRESS 0x5A

/**
 * @brief Mode register address.
 */
#define MODE_REGISTER_ADDRESS 0x01

/**
 * @brief Library selection register address.
 */
#define LIBRARY_SELECTION_REGISTER_ADDRESS 0x03

/**
 * @brief Waveform sequencer register address.
 */
#define WAVEFORM_SEQUENCER_REGISTER_ADDRESS 0x04

/**
 * @brief GO register address.
 */
#define GO_REGISTER_ADDRESS 0x0C

/**
 * @brief Feedback control register address.
 */
#define FEEDBACK_CONTROL_REGISTER_ADDRESS 0x1A

//------------------------------------------------------------------------------
// Function prototypes

static void WriteRegister(const uint8_t address, const uint8_t byte);

//------------------------------------------------------------------------------
// Variables

static const I2CBitBang i2cBitBang = {
    .scl = SCL_HAPTIC_PIN,
    .sda = SDA_HAPTIC_PIN,
    .halfClockCycle = 5, // 100 kHz
};

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
 * @return 0 if successful.
 */
int HapticPlay(const int effect) {
    if ((effect < 0) || (effect > 123)) {
        return 1;
    }
    WriteRegister(WAVEFORM_SEQUENCER_REGISTER_ADDRESS, (uint8_t) effect);
    WriteRegister(GO_REGISTER_ADDRESS, 0x01);
    return 0;
}

/**
 * @brief Write register.
 * @param address Address.
 * @param byte Byte.
 */
static void WriteRegister(const uint8_t address, const uint8_t byte) {
    I2CBitBangStart(&i2cBitBang);
    I2CBitBangSend(&i2cBitBang, I2CClientAddressWrite(I2C_CLIENT_ADDRESS));
    I2CBitBangSend(&i2cBitBang, address);
    I2CBitBangSend(&i2cBitBang, byte);
    I2CBitBangStop(&i2cBitBang);
}

//------------------------------------------------------------------------------
// End of file
