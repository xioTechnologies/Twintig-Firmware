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
#include "Spi/SpiBus2.h"
#include "Spi/SpiBus3.h"
#include "Spi/SpiBus4.h"
#include "Spi/SpiBus5.h"
#include "Spi/SpiBus6.h"

//------------------------------------------------------------------------------
// Definitions

#define ICM1_BUFFER_LENGTH                  (100)
#define ICM1_SPI_BUS_ADD_CLIENT             SpiBus4AddClient
#define ICM1_SPI_BUS_TRANSFER               SpiBus4Transfer
#define ICM1_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus4TransferInProgress
#define ICM1_CS_PIN                         CS1_CH1_PIN
#define ICM1_INT_PIN                        INT1_CH1_PIN

#define ICM2_BUFFER_LENGTH                  (100)
#define ICM2_SPI_BUS_ADD_CLIENT             SpiBus4AddClient
#define ICM2_SPI_BUS_TRANSFER               SpiBus4Transfer
#define ICM2_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus4TransferInProgress
#define ICM2_CS_PIN                         CS2_CH1_PIN
#define ICM2_INT_PIN                        INT2_CH1_PIN

#define ICM3_BUFFER_LENGTH                  (100)
#define ICM3_SPI_BUS_ADD_CLIENT             SpiBus4AddClient
#define ICM3_SPI_BUS_TRANSFER               SpiBus4Transfer
#define ICM3_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus4TransferInProgress
#define ICM3_CS_PIN                         CS3_CH1_PIN
#define ICM3_INT_PIN                        INT3_CH1_PIN

#define ICM4_BUFFER_LENGTH                  (100)
#define ICM4_SPI_BUS_ADD_CLIENT             SpiBus4AddClient
#define ICM4_SPI_BUS_TRANSFER               SpiBus4Transfer
#define ICM4_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus4TransferInProgress
#define ICM4_CS_PIN                         CS4_CH1_PIN
#define ICM4_INT_PIN                        INT4_CH1_PIN

#define ICM5_BUFFER_LENGTH                  (100)
#define ICM5_SPI_BUS_ADD_CLIENT             SpiBus6AddClient
#define ICM5_SPI_BUS_TRANSFER               SpiBus6Transfer
#define ICM5_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus6TransferInProgress
#define ICM5_CS_PIN                         CS1_CH2_PIN
#define ICM5_INT_PIN                        INT1_CH2_PIN

#define ICM6_BUFFER_LENGTH                  (100)
#define ICM6_SPI_BUS_ADD_CLIENT             SpiBus6AddClient
#define ICM6_SPI_BUS_TRANSFER               SpiBus6Transfer
#define ICM6_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus6TransferInProgress
#define ICM6_CS_PIN                         CS2_CH2_PIN
#define ICM6_INT_PIN                        INT2_CH2_PIN

#define ICM7_BUFFER_LENGTH                  (100)
#define ICM7_SPI_BUS_ADD_CLIENT             SpiBus6AddClient
#define ICM7_SPI_BUS_TRANSFER               SpiBus6Transfer
#define ICM7_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus6TransferInProgress
#define ICM7_CS_PIN                         CS3_CH2_PIN
#define ICM7_INT_PIN                        INT3_CH2_PIN

#define ICM8_BUFFER_LENGTH                  (100)
#define ICM8_SPI_BUS_ADD_CLIENT             SpiBus6AddClient
#define ICM8_SPI_BUS_TRANSFER               SpiBus6Transfer
#define ICM8_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus6TransferInProgress
#define ICM8_CS_PIN                         CS4_CH2_PIN
#define ICM8_INT_PIN                        INT4_CH2_PIN

#define ICM9_BUFFER_LENGTH                  (100)
#define ICM9_SPI_BUS_ADD_CLIENT             SpiBus3AddClient
#define ICM9_SPI_BUS_TRANSFER               SpiBus3Transfer
#define ICM9_SPI_BUS_TRANSFER_IN_PROGRESS   SpiBus3TransferInProgress
#define ICM9_CS_PIN                         CS1_CH3_PIN
#define ICM9_INT_PIN                        INT1_CH3_PIN

#define ICM10_BUFFER_LENGTH                 (100)
#define ICM10_SPI_BUS_ADD_CLIENT            SpiBus3AddClient
#define ICM10_SPI_BUS_TRANSFER              SpiBus3Transfer
#define ICM10_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus3TransferInProgress
#define ICM10_CS_PIN                        CS2_CH3_PIN
#define ICM10_INT_PIN                       INT2_CH3_PIN

#define ICM11_BUFFER_LENGTH                 (100)
#define ICM11_SPI_BUS_ADD_CLIENT            SpiBus3AddClient
#define ICM11_SPI_BUS_TRANSFER              SpiBus3Transfer
#define ICM11_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus3TransferInProgress
#define ICM11_CS_PIN                        CS3_CH3_PIN
#define ICM11_INT_PIN                       INT3_CH3_PIN

#define ICM12_BUFFER_LENGTH                 (100)
#define ICM12_SPI_BUS_ADD_CLIENT            SpiBus3AddClient
#define ICM12_SPI_BUS_TRANSFER              SpiBus3Transfer
#define ICM12_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus3TransferInProgress
#define ICM12_CS_PIN                        CS4_CH3_PIN
#define ICM12_INT_PIN                       INT4_CH3_PIN

#define ICM13_BUFFER_LENGTH                 (100)
#define ICM13_SPI_BUS_ADD_CLIENT            SpiBus5AddClient
#define ICM13_SPI_BUS_TRANSFER              SpiBus5Transfer
#define ICM13_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus5TransferInProgress
#define ICM13_CS_PIN                        CS1_CH4_PIN
#define ICM13_INT_PIN                       INT1_CH4_PIN

#define ICM14_BUFFER_LENGTH                 (100)
#define ICM14_SPI_BUS_ADD_CLIENT            SpiBus5AddClient
#define ICM14_SPI_BUS_TRANSFER              SpiBus5Transfer
#define ICM14_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus5TransferInProgress
#define ICM14_CS_PIN                        CS2_CH4_PIN
#define ICM14_INT_PIN                       INT2_CH4_PIN

#define ICM15_BUFFER_LENGTH                 (100)
#define ICM15_SPI_BUS_ADD_CLIENT            SpiBus5AddClient
#define ICM15_SPI_BUS_TRANSFER              SpiBus5Transfer
#define ICM15_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus5TransferInProgress
#define ICM15_CS_PIN                        CS3_CH4_PIN
#define ICM15_INT_PIN                       INT3_CH4_PIN

#define ICM16_BUFFER_LENGTH                 (100)
#define ICM16_SPI_BUS_ADD_CLIENT            SpiBus5AddClient
#define ICM16_SPI_BUS_TRANSFER              SpiBus5Transfer
#define ICM16_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus5TransferInProgress
#define ICM16_CS_PIN                        CS4_CH4_PIN
#define ICM16_INT_PIN                       INT4_CH4_PIN

#define ICM17_BUFFER_LENGTH                 (100)
#define ICM17_SPI_BUS_ADD_CLIENT            SpiBus2AddClient
#define ICM17_SPI_BUS_TRANSFER              SpiBus2Transfer
#define ICM17_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus2TransferInProgress
#define ICM17_CS_PIN                        CS1_CH5_PIN
#define ICM17_INT_PIN                       INT1_CH5_PIN

#define ICM18_BUFFER_LENGTH                 (100)
#define ICM18_SPI_BUS_ADD_CLIENT            SpiBus2AddClient
#define ICM18_SPI_BUS_TRANSFER              SpiBus2Transfer
#define ICM18_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus2TransferInProgress
#define ICM18_CS_PIN                        CS2_CH5_PIN
#define ICM18_INT_PIN                       INT2_CH5_PIN

#define ICM19_BUFFER_LENGTH                 (100)
#define ICM19_SPI_BUS_ADD_CLIENT            SpiBus2AddClient
#define ICM19_SPI_BUS_TRANSFER              SpiBus2Transfer
#define ICM19_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus2TransferInProgress
#define ICM19_CS_PIN                        CS3_CH5_PIN
#define ICM19_INT_PIN                       INT3_CH5_PIN

#define ICM20_BUFFER_LENGTH                 (100)
#define ICM20_SPI_BUS_ADD_CLIENT            SpiBus2AddClient
#define ICM20_SPI_BUS_TRANSFER              SpiBus2Transfer
#define ICM20_SPI_BUS_TRANSFER_IN_PROGRESS  SpiBus2TransferInProgress
#define ICM20_CS_PIN                        CS4_CH5_PIN
#define ICM20_INT_PIN                       INT4_CH5_PIN

#endif

//------------------------------------------------------------------------------
// End of file
