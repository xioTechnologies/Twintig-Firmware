/**
 * @file I2CPrint.h
 * @author Seb Madgwick
 * @brief I2C driver for PIC32 devices.
 */

#ifndef I2C_PRINT_H
#define I2C_PRINT_H

//------------------------------------------------------------------------------
// Includes

#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Function declarations

void I2CPrintStart(void);
void I2CPrintRepeatedStart(void);
void I2CPrintStop(void);
void I2CPrintByte(const uint8_t byte);
void I2CPrintReadAddress(const uint8_t address);
void I2CPrintWriteAddress(const uint8_t address);
void I2CPrintAckNack(const bool ack);

#endif

//------------------------------------------------------------------------------
// End of file
