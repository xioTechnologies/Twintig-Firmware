/**
 * @file I2CBB4.h
 * @author Seb Madgwick
 * @brief I2C bit-bang driver.
 */

#ifndef I2CBB4_H
#define I2CBB4_H

//------------------------------------------------------------------------------
// Includes

#include "I2C.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const I2C i2cBB4;

//------------------------------------------------------------------------------
// Function declarations

void I2CBB4BusClear(void);
void I2CBB4Start(void);
void I2CBB4RepeatedStart(void);
void I2CBB4Stop(void);
bool I2CBB4Send(const uint8_t byte);
bool I2CBB4SendAddressRead(const uint8_t address);
bool I2CBB4SendAddressWrite(const uint8_t address);
uint8_t I2CBB4Receive(const bool ack);

#endif

//------------------------------------------------------------------------------
// End of file
