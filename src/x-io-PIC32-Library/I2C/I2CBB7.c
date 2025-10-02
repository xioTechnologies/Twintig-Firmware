/**
 * @file I2CBB7.c
 * @author Seb Madgwick
 * @brief I2C bit-bang driver.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
#include "I2CBB.h"
#include "I2CBB7.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Uncomment this line to print messages.
 */
//#define PRINT_MESSAGES

//------------------------------------------------------------------------------
// Variables

const I2C i2cBB7 = {
    .start = I2CBB7Start,
    .repeatedStart = I2CBB7RepeatedStart,
    .stop = I2CBB7Stop,
    .send = I2CBB7Send,
    .sendAddressRead = I2CBB7SendAddressRead,
    .sendAddressWrite = I2CBB7SendAddressWrite,
    .receive = I2CBB7Receive,
};
static const I2CBB i2cBB = {
    .sclPin = I2CBB7_SCL_PIN,
    .sdaPin = I2CBB7_SDA_PIN,
    .halfClockCycle = I2CBB7_HALF_CLOCK_CYCLE,
};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Performs the bus clear procedure. This procedure should be performed
 * if the SDA line is stuck low.
 * See page 20 of UM10204 I2C-bus specification and user manual Rev. 6 - 4 April
 * 2014.
 * @param i2cBB I2C bit bang structure.
 */
void I2CBB7BusClear(void) {
    I2CBBBusClear(&i2cBB);
}

/**
 * @brief Generates a start event.
 */
void I2CBB7Start(void) {
    I2CBBStart(&i2cBB);
#ifdef PRINT_MESSAGES
    I2CPrintStart();
#endif
}

/**
 * @brief Generates a repeated start event.
 */
void I2CBB7RepeatedStart(void) {
    I2CBBRepeatedStart(&i2cBB);
#ifdef PRINT_MESSAGES
    I2CPrintRepeatedStart();
#endif
}

/**
 * @brief Generates a stop event.
 */
void I2CBB7Stop(void) {
    I2CBBStop(&i2cBB);
#ifdef PRINT_MESSAGES
    I2CPrintStop();
#endif
}

/**
 * @brief Sends a byte and checks for ACK.
 * @param byte Byte.
 * @return True if an ACK was generated.
 */
bool I2CBB7Send(const uint8_t byte) {
    const bool ack = I2CBBSend(&i2cBB, byte);
#ifdef PRINT_MESSAGES
    I2CPrintByte(byte);
    I2CPrintAckNack(ack);
#endif
    return ack;
}

/**
 * @brief Sends a 7-bit client address with appended R/W bit to indicate a
 * read.
 * @param address 7-bit client address.
 * @return True if an ACK was generated.
 */
bool I2CBB7SendAddressRead(const uint8_t address) {
    const bool ack = I2CBBSendAddressRead(&i2cBB, address);
#ifdef PRINT_MESSAGES
    I2CPrintReadAddress(address);
    I2CPrintAckNack(ack);
#endif
    return ack;
}

/**
 * @brief Sends a 7-bit client address with appended R/W bit to indicate a
 * write.
 * @param address 7-bit client address.
 * @return True if an ACK was generated.
 */
bool I2CBB7SendAddressWrite(const uint8_t address) {
    const bool ack = I2CBBSendAddressWrite(&i2cBB, address);
#ifdef PRINT_MESSAGES
    I2CPrintWriteAddress(address);
    I2CPrintAckNack(ack);
#endif
    return ack;
}

/**
 * @brief Receives a byte and generates an ACK or NACK.
 * @param ack True for ACK.
 * @return Byte.
 */
uint8_t I2CBB7Receive(const bool ack) {
    const uint8_t byte = I2CBBReceive(&i2cBB, ack);
#ifdef PRINT_MESSAGES
    I2CPrintByte(byte);
    I2CPrintAckNack(ack);
#endif
    return byte;
}

//------------------------------------------------------------------------------
// End of file
