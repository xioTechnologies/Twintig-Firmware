/**
 * @file SpiBus6.h
 * @author Seb Madgwick
 * @brief SPI bus.
 */

#ifndef SPI_BUS_6_H
#define SPI_BUS_6_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "SpiBus.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Function declarations

SpiBusClient * const SpiBus6AddClient(const GPIO_PIN csPin);
void SpiBus6Transfer(SpiBusClient * const client, void* const data, const size_t numberOfBytes, void (*transferComplete)(void));
bool SpiBus6TransferInProgress(const SpiBusClient * const client);

#endif

//------------------------------------------------------------------------------
// End of file
