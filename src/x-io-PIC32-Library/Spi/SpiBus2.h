/**
 * @file SpiBus2.h
 * @author Seb Madgwick
 * @brief Manages access to the SPI bus.
 */

#ifndef SPI_BUS_2_H
#define SPI_BUS_2_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "SpiBus.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Function declarations

SpiBusClient * const SpiBus2AddClient(const GPIO_PIN csPin);
void SpiBus2Transfer(SpiBusClient * const client, void* const data, const size_t numberOfBytes, void (*transferComplete)(void));
bool SpiBus2TransferInProgress(const SpiBusClient * const client);

#endif

//------------------------------------------------------------------------------
// End of file
