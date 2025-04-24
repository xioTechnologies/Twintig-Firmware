/**
 * @file I2CPrint.c
 * @author Seb Madgwick
 * @brief I2C driver for PIC32 devices.
 */

//------------------------------------------------------------------------------
// Includes

#include "I2C.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Print start event.
 */
void I2CPrintStart(void) {
    printf("S ");
}

/**
 * @brief Print repeated start event.
 */
void I2CPrintRepeatedStart(void) {
    printf("R ");
}

/**
 * @brief Print stop event.
 */
void I2CPrintStop(void) {
    printf("P\n");
}

/**
 * @brief Print byte.
 * @param byte Byte.
 */
void I2CPrintByte(const uint8_t byte) {
    printf("%02X", byte);
}

/**
 * @brief Print 7-bit client address used to read.
 * @param address 7-bit client address.
 */
void I2CPrintReadAddress(const uint8_t address) {
    printf("r%02X", address);
}

/**
 * @brief Print 7-bit client address used to write.
 * @param address 7-bit client address.
 */
void I2CPrintWriteAddress(const uint8_t address) {
    printf("w%02X", address);
}

/**
 * @brief Print ACK or NACK.
 * @param ack True for ACK.
 */
void I2CPrintAckNack(const bool ack) {
    printf("%c ", ack ? '-' : '^');
}

//------------------------------------------------------------------------------
// End of file
