/**
 * @file SpiBus1.h
 * @author Seb Madgwick
 * @brief Manages access to the SPI bus.
 */

#ifndef SPI_BUS_1_H
#define SPI_BUS_1_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "SpiBus.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Function declarations

SpiBusClient SpiBus1AddClient(const GPIO_PIN csPin);
void SpiBus1Transfer(const SpiBusClient client, void* const data, const size_t numberOfBytes, void (*transferComplete)(void));
bool SpiBus1TransferInProgress(const SpiBusClient client);

#endif

//------------------------------------------------------------------------------
// End of file
