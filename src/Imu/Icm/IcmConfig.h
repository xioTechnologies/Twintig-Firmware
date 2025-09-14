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

#define ICM1_SPI_BUS        spiBus4
#define ICM1_CS_PIN         CS1_CH1_PIN
#define ICM1_INT_PIN        INT1_CH1_PIN

#define ICM2_SPI_BUS        spiBus4
#define ICM2_CS_PIN         CS2_CH1_PIN
#define ICM2_INT_PIN        INT2_CH1_PIN

#define ICM3_SPI_BUS        spiBus4
#define ICM3_CS_PIN         CS3_CH1_PIN
#define ICM3_INT_PIN        INT3_CH1_PIN

#define ICM4_SPI_BUS        spiBus4
#define ICM4_CS_PIN         CS4_CH1_PIN
#define ICM4_INT_PIN        INT4_CH1_PIN

#define ICM5_SPI_BUS        spiBus6
#define ICM5_CS_PIN         CS1_CH2_PIN
#define ICM5_INT_PIN        INT1_CH2_PIN

#define ICM6_SPI_BUS        spiBus6
#define ICM6_CS_PIN         CS2_CH2_PIN
#define ICM6_INT_PIN        INT2_CH2_PIN

#define ICM7_SPI_BUS        spiBus6
#define ICM7_CS_PIN         CS3_CH2_PIN
#define ICM7_INT_PIN        INT3_CH2_PIN

#define ICM8_SPI_BUS        spiBus6
#define ICM8_CS_PIN         CS4_CH2_PIN
#define ICM8_INT_PIN        INT4_CH2_PIN

#define ICM9_SPI_BUS        spiBus3
#define ICM9_CS_PIN         CS1_CH3_PIN
#define ICM9_INT_PIN        INT1_CH3_PIN

#define ICM10_SPI_BUS       spiBus3
#define ICM10_CS_PIN        CS2_CH3_PIN
#define ICM10_INT_PIN       INT2_CH3_PIN

#define ICM11_SPI_BUS       spiBus3
#define ICM11_CS_PIN        CS3_CH3_PIN
#define ICM11_INT_PIN       INT3_CH3_PIN

#define ICM12_SPI_BUS       spiBus3
#define ICM12_CS_PIN        CS4_CH3_PIN
#define ICM12_INT_PIN       INT4_CH3_PIN

#define ICM13_SPI_BUS       spiBus5
#define ICM13_CS_PIN        CS1_CH4_PIN
#define ICM13_INT_PIN       INT1_CH4_PIN

#define ICM14_SPI_BUS       spiBus5
#define ICM14_CS_PIN        CS2_CH4_PIN
#define ICM14_INT_PIN       INT2_CH4_PIN

#define ICM15_SPI_BUS       spiBus5
#define ICM15_CS_PIN        CS3_CH4_PIN
#define ICM15_INT_PIN       INT3_CH4_PIN

#define ICM16_SPI_BUS       spiBus5
#define ICM16_CS_PIN        CS4_CH4_PIN
#define ICM16_INT_PIN       INT4_CH4_PIN

#define ICM17_SPI_BUS       spiBus2
#define ICM17_CS_PIN        CS1_CH5_PIN
#define ICM17_INT_PIN       INT1_CH5_PIN

#define ICM18_SPI_BUS       spiBus2
#define ICM18_CS_PIN        CS2_CH5_PIN
#define ICM18_INT_PIN       INT2_CH5_PIN

#define ICM19_SPI_BUS       spiBus2
#define ICM19_CS_PIN        CS3_CH5_PIN
#define ICM19_INT_PIN       INT3_CH5_PIN

#define ICM20_SPI_BUS       spiBus2
#define ICM20_CS_PIN        CS4_CH5_PIN
#define ICM20_INT_PIN       INT4_CH5_PIN

#endif

//------------------------------------------------------------------------------
// End of file
