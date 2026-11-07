/**
 * @file I2CBB6.h
 * @author Seb Madgwick
 * @brief I2C bit-bang driver.
 */

#ifndef I2CBB6_H
#define I2CBB6_H

//------------------------------------------------------------------------------
// Includes

#include "I2C.h"
#include <stdbool.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const I2C i2cBB6;

//------------------------------------------------------------------------------
// Function declarations

void I2CBB6BusClear(void);
void I2CBB6Start(void);
void I2CBB6RepeatedStart(void);
void I2CBB6Stop(void);
bool I2CBB6Transmit(const uint8_t byte);
bool I2CBB6TransmitAddressRead(const uint8_t address);
bool I2CBB6TransmitAddressWrite(const uint8_t address);
uint8_t I2CBB6ReceiveAck(void);
uint8_t I2CBB6ReceiveNack(void);

#endif

//------------------------------------------------------------------------------
// End of file
