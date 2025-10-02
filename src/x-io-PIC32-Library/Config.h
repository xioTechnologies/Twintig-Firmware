/**
 * @file Config.h
 * @author Seb Madgwick
 * @brief Library configuration.
 */

#ifndef CONFIG_H
#define CONFIG_H

//------------------------------------------------------------------------------
// Includes

#include "definitions.h"
#include "Spi/Spi1DmaTx.h"
#include "Spi/Spi2.h"
#include "Spi/Spi3Dma.h"
#include "Spi/Spi4Dma.h"
#include "Spi/Spi5.h"
#include "Spi/Spi6Dma.h"

//------------------------------------------------------------------------------
// Definitions

#define EEPROM_I2C_ADDRESS                  (0x50)
#define EEPROM_SIZE                         (0x1000)
#define EEPROM_PAGE_SIZE                    (32)

#define I2CBB1_SCL_PIN                      SCL_EEPROM_PIN
#define I2CBB1_SDA_PIN                      SDA_EEPROM_PIN
#define I2CBB1_HALF_CLOCK_CYCLE             (5)

#define I2CBB2_SCL_PIN                      SCL_HAPTIC_PIN
#define I2CBB2_SDA_PIN                      SDA_HAPTIC_PIN
#define I2CBB2_HALF_CLOCK_CYCLE             (5)

#define I2CBB3_SCL_PIN                      SCL_CH1_PIN
#define I2CBB3_SDA_PIN                      SDA_CH1_PIN
#define I2CBB3_HALF_CLOCK_CYCLE             (5)

#define I2CBB4_SCL_PIN                      SCL_CH2_PIN
#define I2CBB4_SDA_PIN                      SDA_CH2_PIN
#define I2CBB4_HALF_CLOCK_CYCLE             (5)

#define I2CBB5_SCL_PIN                      SCL_CH3_PIN
#define I2CBB5_SDA_PIN                      SDA_CH3_PIN
#define I2CBB5_HALF_CLOCK_CYCLE             (5)

#define I2CBB6_SCL_PIN                      SCL_CH4_PIN
#define I2CBB6_SDA_PIN                      SDA_CH4_PIN
#define I2CBB6_HALF_CLOCK_CYCLE             (5)

#define I2CBB7_SCL_PIN                      SCL_CH5_PIN
#define I2CBB7_SDA_PIN                      SDA_CH5_PIN
#define I2CBB7_HALF_CLOCK_CYCLE             (5)

#define NEOPIXELS_1_HAL_NUMBER_OF_PIXELS    (2)

#define NEOPIXELS_2_HAL_NUMBER_OF_PIXELS    (4)

#define NEOPIXELS_3_HAL_NUMBER_OF_PIXELS    (4)

#define NEOPIXELS_4_HAL_NUMBER_OF_PIXELS    (4)

#define NEOPIXELS_5_HAL_NUMBER_OF_PIXELS    (4)

#define NEOPIXELS_6_HAL_NUMBER_OF_PIXELS    (4)

#define SPI1_CS_ACTIVE_HIGH

#define SPI_BUS_1_MAX_NUMBER_OF_CLIENTS     (6)
#define SPI_BUS_1_SPI                       spi1DmaTx

#define SPI_BUS_2_MAX_NUMBER_OF_CLIENTS     (4)
#define SPI_BUS_2_SPI                       spi2

#define SPI_BUS_3_MAX_NUMBER_OF_CLIENTS     (4)
#define SPI_BUS_3_SPI                       spi3Dma

#define SPI_BUS_4_MAX_NUMBER_OF_CLIENTS     (4)
#define SPI_BUS_4_SPI                       spi4Dma

#define SPI_BUS_5_MAX_NUMBER_OF_CLIENTS     (4)
#define SPI_BUS_5_SPI                       spi5

#define SPI_BUS_6_MAX_NUMBER_OF_CLIENTS     (4)
#define SPI_BUS_6_SPI                       spi6Dma

#define UART1_READ_BUFFER_SIZE              (4096)

#define UART3_READ_BUFFER_SIZE              (16)
#define UART3_WRITE_BUFFER_SIZE             (4096)

#define USB_CDC_READ_BUFFER_SIZE            (4096)
#define USB_CDC_WRITE_BUFFER_SIZE           (20 * 4096)

#endif

//------------------------------------------------------------------------------
// End of file
