/**
 * @file I2CBB5.h
 * @author Seb Madgwick
 * @brief I2C bit-bang driver.
 */

#ifndef I2CBB5_H
#define I2CBB5_H

//------------------------------------------------------------------------------
// Includes

#include "I2C.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const I2C i2cBB5;

//------------------------------------------------------------------------------
// Function declarations

void I2CBB5BusClear(void);
void I2CBB5Start(void);
void I2CBB5RepeatedStart(void);
void I2CBB5Stop(void);
bool I2CBB5Transmit(const uint8_t byte);
bool I2CBB5TransmitAddressRead(const uint8_t address);
bool I2CBB5TransmitAddressWrite(const uint8_t address);
uint8_t I2CBB5ReceiveAck(void);
uint8_t I2CBB5ReceiveNack(void);

#endif

//------------------------------------------------------------------------------
// End of file
