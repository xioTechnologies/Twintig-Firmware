/**
 * @file SpiBusConfig.h
 * @author Seb Madgwick
 * @brief Manages access to the SPI bus.
 */

#ifndef SPI_BUS_CONFIG_H
#define SPI_BUS_CONFIG_H

//------------------------------------------------------------------------------
// Includes

#include "Spi/Spi1DmaTx.h"
#include "Spi/Spi2.h"
#include "Spi/Spi3Dma.h"
#include "Spi/Spi4Dma.h"
#include "Spi/Spi5.h"
#include "Spi/Spi6Dma.h"

//------------------------------------------------------------------------------
// Definitions

#define SPI_BUS_1_MAX_NUMBER_OF_CLIENTS (4)
#define SPI_BUS_1_TRANSFER              Spi1DmaTxTransfer

#define SPI_BUS_2_MAX_NUMBER_OF_CLIENTS (4)
#define SPI_BUS_2_TRANSFER              Spi2Transfer

#define SPI_BUS_3_MAX_NUMBER_OF_CLIENTS (4)
#define SPI_BUS_3_TRANSFER              Spi3DmaTransfer

#define SPI_BUS_4_MAX_NUMBER_OF_CLIENTS (4)
#define SPI_BUS_4_TRANSFER              Spi4DmaTransfer

#define SPI_BUS_5_MAX_NUMBER_OF_CLIENTS (4)
#define SPI_BUS_5_TRANSFER              Spi5Transfer

#define SPI_BUS_6_MAX_NUMBER_OF_CLIENTS (4)
#define SPI_BUS_6_TRANSFER              Spi6DmaTransfer

#endif

//------------------------------------------------------------------------------
// End of file
