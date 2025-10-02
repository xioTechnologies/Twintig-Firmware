/**
 * @file I2CBB3.h
 * @author Seb Madgwick
 * @brief I2C bit-bang driver.
 */

#ifndef I2CBB3_H
#define I2CBB3_H

//------------------------------------------------------------------------------
// Includes

#include "I2C.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const I2C i2cBB3;

//------------------------------------------------------------------------------
// Function declarations

void I2CBB3BusClear(void);
void I2CBB3Start(void);
void I2CBB3RepeatedStart(void);
void I2CBB3Stop(void);
bool I2CBB3Send(const uint8_t byte);
bool I2CBB3SendAddressRead(const uint8_t address);
bool I2CBB3SendAddressWrite(const uint8_t address);
uint8_t I2CBB3Receive(const bool ack);

#endif

//------------------------------------------------------------------------------
// End of file
