/**
 * @file SpiBus5.h
 * @author Seb Madgwick
 * @brief Manages access to the SPI bus.
 */

#ifndef SPI_BUS_5_H
#define SPI_BUS_5_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "SpiBus.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Function declarations

SpiBusClient SpiBus5AddClient(const GPIO_PIN csPin);
void SpiBus5Transfer(const SpiBusClient client, void* const data, const size_t numberOfBytes, void (*transferComplete)(void));
bool SpiBus5TransferInProgress(const SpiBusClient client);

#endif

//------------------------------------------------------------------------------
// End of file
