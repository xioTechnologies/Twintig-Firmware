/**
 * @file Spi6Dma.h
 * @author Seb Madgwick
 * @brief SPI driver using DMA for PIC32 devices.
 */

#ifndef SPI6_DMA_H
#define SPI6_DMA_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Spi.h"
#include <stdbool.h>
#include <stddef.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Spi spi6Dma;

//------------------------------------------------------------------------------
// Function declarations

void Spi6DmaInitialise(const SpiSettings * const settings);
void Spi6DmaDeinitialise(void);
void Spi6DmaTransfer(const GPIO_PIN csPin_, volatile void* const data_, const size_t numberOfBytes_, void (*const transferComplete_) (void));
bool Spi6DmaTransferInProgress(void);

#endif

//------------------------------------------------------------------------------
// End of file
