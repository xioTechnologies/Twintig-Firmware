/**
 * @file I2CBB7.h
 * @author Seb Madgwick
 * @brief I2C bit-bang driver.
 */

#ifndef I2CBB7_H
#define I2CBB7_H

//------------------------------------------------------------------------------
// Includes

#include "I2C.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const I2C i2cBB7;

//------------------------------------------------------------------------------
// Function declarations

void I2CBB7BusClear(void);
void I2CBB7Start(void);
void I2CBB7RepeatedStart(void);
void I2CBB7Stop(void);
bool I2CBB7Send(const uint8_t byte);
bool I2CBB7SendAddressRead(const uint8_t address);
bool I2CBB7SendAddressWrite(const uint8_t address);
uint8_t I2CBB7Receive(const bool ack);

#endif

//------------------------------------------------------------------------------
// End of file
