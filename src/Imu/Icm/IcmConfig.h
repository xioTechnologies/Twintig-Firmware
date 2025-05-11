/**
 * @file IcmConfig.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM_CONFIG_H
#define ICM_CONFIG_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Spi/Spi2.h"
#include "Spi/Spi3Dma.h"
#include "Spi/Spi4Dma.h"
#include "Spi/Spi5.h"
#include "Spi/Spi6Dma.h"

//------------------------------------------------------------------------------
// Definitions

#define ICM1_SPI_TRANSFER               Spi4DmaTransfer
#define ICM1_SPI_TRANSFER_IN_PROGRESS   Spi4DmaTransferInProgress
#define ICM1_CS_PIN                     CS4_CH1_PIN
#define ICM1_INT_PIN                    INT4_CH1_PIN

#define ICM2_SPI_TRANSFER               Spi6DmaTransfer
#define ICM2_SPI_TRANSFER_IN_PROGRESS   Spi6DmaTransferInProgress
#define ICM2_CS_PIN                     CS4_CH2_PIN
#define ICM2_INT_PIN                    INT4_CH2_PIN

#define ICM3_SPI_TRANSFER               Spi3DmaTransfer
#define ICM3_SPI_TRANSFER_IN_PROGRESS   Spi3DmaTransferInProgress
#define ICM3_CS_PIN                     CS4_CH3_PIN
#define ICM3_INT_PIN                    INT4_CH3_PIN

#define ICM4_SPI_TRANSFER               Spi5Transfer
#define ICM4_SPI_TRANSFER_IN_PROGRESS   Spi5TransferInProgress
#define ICM4_CS_PIN                     CS4_CH4_PIN
#define ICM4_INT_PIN                    INT4_CH4_PIN

#define ICM5_SPI_TRANSFER               Spi2Transfer
#define ICM5_SPI_TRANSFER_IN_PROGRESS   Spi2TransferInProgress
#define ICM5_CS_PIN                     CS4_CH5_PIN
#define ICM5_INT_PIN                    INT4_CH5_PIN

#endif

//------------------------------------------------------------------------------
// End of file
