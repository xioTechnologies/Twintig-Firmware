/**
 * @file Spi6Dma.c
 * @author Seb Madgwick
 * @brief SPI driver using DMA for PIC32 devices.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
#include "definitions.h"
#include "Spi6Dma.h"
#include "sys/kmem.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Uncomment this line to print transfers.
 */
//#define PRINT_TRANSFERS

//------------------------------------------------------------------------------
// Variables

const Spi spi6Dma = {
    .transfer = Spi6DmaTransfer,
    .transferInProgress = Spi6DmaTransferInProgress,
};
static GPIO_PIN csPin;
#ifdef PRINT_TRANSFERS
static void* data;
static size_t numberOfBytes;
#endif
static void (*transferComplete)(void);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module.
 * @param settings Settings.
 */
void Spi6DmaInitialise(const SpiSettings * const settings) {

    // Ensure default register states
    Spi6DmaDeinitialise();

    // Configure SPI
    SPI6CONbits.MSTEN = 1; // host mode
    SPI6CONbits.ENHBUF = 1; // enhanced Buffer mode is enabled
    SPI6CONbits.SMP = 1; // input data sampled at end of data output time
    SPI6CONbits.CKP = settings->clockPolarity;
    SPI6CONbits.CKE = settings->clockPhase;
    SPI6CONbits.STXISEL = 0b11; // interrupt is generated when the buffer is not full (has one or more empty elements)
    SPI6CONbits.SRXISEL = 0b01; // interrupt is generated when the buffer is not empty
    SPI6BRG = SpiCalculateSpixbrg(settings->clockFrequency);
    SPI6CONbits.ON = 1;

    // Enable DMA
    DMACONbits.ON = 1;

    // Configure TX DMA channel
#ifdef _SPI6_TX_IRQ
    DCH6ECONbits.CHSIRQ = _SPI6_TX_IRQ;
#else
    DCH6ECONbits.CHSIRQ = _SPI6_TX_VECTOR;
#endif
    DCH6ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH6DSA = KVA_TO_PA(&SPI6BUF); // destination address
    DCH6DSIZ = 1; // destination size
    DCH6CSIZ = 1; // transfers per event

    // Configure RX DMA channel
#ifdef _SPI6_RX_IRQ
    DCH7ECONbits.CHSIRQ = _SPI6_RX_IRQ;
#else
    DCH7ECONbits.CHSIRQ = _SPI6_RX_VECTOR;
#endif
    DCH7ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH7SSA = KVA_TO_PA(&SPI6BUF); // source address
    DCH7SSIZ = 1; // source size
    DCH7CSIZ = 1; // transfers per event
    DCH7INTbits.CHBCIE = 1; // channel Block Transfer Complete Interrupt Enable bit

    // Configure RX DMA channel interrupt
    EVIC_SourceEnable(INT_SOURCE_DMA7);
}

/**
 * @brief Deinitialises the module.
 */
void Spi6DmaDeinitialise(void) {

    // Disable SPI and restore default register states
    SPI6CON = 0;
    SPI6CON2 = 0;
    SPI6STAT = 0;
    SPI6BRG = 0;

    // Disable TX DMA channel and restore default register states
    DCH6CON = 0;
    DCH6ECON = 0;
    DCH6INT = 0;
    DCH6SSA = 0;
    DCH6DSA = 0;
    DCH6SSIZ = 0;
    DCH6DSIZ = 0;
    DCH6SPTR = 0;
    DCH6DPTR = 0;
    DCH6CSIZ = 0;
    DCH6CPTR = 0;
    DCH6DAT = 0;

    // Disable RX DMA channel and restore default register states
    DCH7CON = 0;
    DCH7ECON = 0;
    DCH7INT = 0;
    DCH7SSA = 0;
    DCH7DSA = 0;
    DCH7SSIZ = 0;
    DCH7DSIZ = 0;
    DCH7SPTR = 0;
    DCH7DPTR = 0;
    DCH7CSIZ = 0;
    DCH7CPTR = 0;
    DCH7DAT = 0;

    // Disable interrupt
    EVIC_SourceDisable(INT_SOURCE_DMA7);
    EVIC_SourceStatusClear(INT_SOURCE_DMA7);
}

/**
 * @brief Transfers data. The data will be overwritten with the received data.
 * The data must be declared __attribute__((coherent)) for PIC32MZ devices.
 * This function must not be called while a transfer is in progress. The
 * transfer complete callback will be called from within an interrupt once the
 * transfer is complete.
 * @param csPin_ CS pin.
 * @param data_ Data.
 * @param numberOfBytes_ Number of bytes.
 * @param transferComplete_ Transfer complete callback.
 */
void Spi6DmaTransfer(const GPIO_PIN csPin_, volatile void* const data_, const size_t numberOfBytes_, void (*const transferComplete_) (void)) {

    // Store arguments
    csPin = csPin_;
#ifdef PRINT_TRANSFERS
    data = (uint8_t*) data_;
    numberOfBytes = numberOfBytes_;
#endif
    transferComplete = transferComplete_;

    // Print
#ifdef PRINT_TRANSFERS
    SpiPrintTransfer(csPin, data, numberOfBytes);
#endif

    // Configure TX DMA channel
    DCH6SSA = KVA_TO_PA(data_); // source address
    DCH6SSIZ = numberOfBytes_; // source size

    // Configure RX DMA channel
    DCH7DSA = KVA_TO_PA(data_); // destination address
    DCH7DSIZ = numberOfBytes_; // destination size

    // Begin transfer
    if (csPin != GPIO_PIN_NONE) {
#ifdef SPI6_CS_ACTIVE_HIGH
        GPIO_PinSet(csPin);
#else
        GPIO_PinClear(csPin);
#endif
    }
    DCH7INTbits.CHBCIF = 0; // clear RX DMA channel interrupt flag
    DCH7CONbits.CHEN = 1; // enable RX DMA channel
    DCH6CONbits.CHEN = 1; // enable TX DMA channel to begin transfer
}

/**
 * @brief DMA interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Dma7InterruptHandler(void) {
    EVIC_SourceStatusClear(INT_SOURCE_DMA7); // clear interrupt flag first because transfer complete callback may start new transfer
    if (csPin != GPIO_PIN_NONE) {
#ifdef SPI6_CS_ACTIVE_HIGH
        GPIO_PinClear(csPin);
#else
        GPIO_PinSet(csPin);
#endif
    }
#ifdef PRINT_TRANSFERS
    SpiPrintTransferComplete(data, numberOfBytes);
#endif
    if (transferComplete != NULL) {
        transferComplete();
    }
}

/**
 * @brief Returns true while the transfer is in progress.
 * @return True while the transfer is in progress.
 */
bool Spi6DmaTransferInProgress(void) {
    return DCH7CONbits.CHEN == 1; // if RX DMA channel interrupt enabled
}

//------------------------------------------------------------------------------
// End of file
