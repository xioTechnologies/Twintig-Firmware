/**
 * @file Spi4Dma.c
 * @author Seb Madgwick
 * @brief SPI driver using DMA for PIC32 devices.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
#include "definitions.h"
#include "Spi4Dma.h"
#include "sys/kmem.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Uncomment this line to print transfers.
 */
//#define PRINT_TRANSFERS

//------------------------------------------------------------------------------
// Variables

const Spi spi4Dma = {
    .transfer = Spi4DmaTransfer,
    .transferInProgress = Spi4DmaTransferInProgress,
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
void Spi4DmaInitialise(const SpiSettings * const settings) {

    // Ensure default register states
    Spi4DmaDeinitialise();

    // Configure SPI
    SPI4CONbits.MSTEN = 1; // host mode
    SPI4CONbits.ENHBUF = 1; // enhanced Buffer mode is enabled
    SPI4CONbits.SMP = 1; // input data sampled at end of data output time
    SPI4CONbits.CKP = settings->clockPolarity;
    SPI4CONbits.CKE = settings->clockPhase;
    SPI4CONbits.STXISEL = 0b11; // interrupt is generated when the buffer is not full (has one or more empty elements)
    SPI4CONbits.SRXISEL = 0b01; // interrupt is generated when the buffer is not empty
    SPI4BRG = SpiCalculateSpixbrg(settings->clockFrequency);
    SPI4CONbits.ON = 1;

    // Enable DMA
    DMACONbits.ON = 1;

    // Configure TX DMA channel
#ifdef _SPI4_TX_IRQ
    DCH4ECONbits.CHSIRQ = _SPI4_TX_IRQ;
#else
    DCH4ECONbits.CHSIRQ = _SPI4_TX_VECTOR;
#endif
    DCH4ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH4DSA = KVA_TO_PA(&SPI4BUF); // destination address
    DCH4DSIZ = 1; // destination size
    DCH4CSIZ = 1; // transfers per event

    // Configure RX DMA channel
#ifdef _SPI4_RX_IRQ
    DCH5ECONbits.CHSIRQ = _SPI4_RX_IRQ;
#else
    DCH5ECONbits.CHSIRQ = _SPI4_RX_VECTOR;
#endif
    DCH5ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH5SSA = KVA_TO_PA(&SPI4BUF); // source address
    DCH5SSIZ = 1; // source size
    DCH5CSIZ = 1; // transfers per event
    DCH5INTbits.CHBCIE = 1; // channel Block Transfer Complete Interrupt Enable bit

    // Configure RX DMA channel interrupt
    EVIC_SourceEnable(INT_SOURCE_DMA5);
}

/**
 * @brief Deinitialises the module.
 */
void Spi4DmaDeinitialise(void) {

    // Disable SPI and restore default register states
    SPI4CON = 0;
    SPI4CON2 = 0;
    SPI4STAT = 0;
    SPI4BRG = 0;

    // Disable TX DMA channel and restore default register states
    DCH4CON = 0;
    DCH4ECON = 0;
    DCH4INT = 0;
    DCH4SSA = 0;
    DCH4DSA = 0;
    DCH4SSIZ = 0;
    DCH4DSIZ = 0;
    DCH4SPTR = 0;
    DCH4DPTR = 0;
    DCH4CSIZ = 0;
    DCH4CPTR = 0;
    DCH4DAT = 0;

    // Disable RX DMA channel and restore default register states
    DCH5CON = 0;
    DCH5ECON = 0;
    DCH5INT = 0;
    DCH5SSA = 0;
    DCH5DSA = 0;
    DCH5SSIZ = 0;
    DCH5DSIZ = 0;
    DCH5SPTR = 0;
    DCH5DPTR = 0;
    DCH5CSIZ = 0;
    DCH5CPTR = 0;
    DCH5DAT = 0;

    // Disable interrupt
    EVIC_SourceDisable(INT_SOURCE_DMA5);
    EVIC_SourceStatusClear(INT_SOURCE_DMA5);
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
 * @param transferComplete_ Transfer complete callback. NULL if unused.
 */
void Spi4DmaTransfer(const GPIO_PIN csPin_, volatile void* const data_, const size_t numberOfBytes_, void (*const transferComplete_) (void)) {

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
    DCH4SSA = KVA_TO_PA(data_); // source address
    DCH4SSIZ = numberOfBytes_; // source size

    // Configure RX DMA channel
    DCH5DSA = KVA_TO_PA(data_); // destination address
    DCH5DSIZ = numberOfBytes_; // destination size

    // Begin transfer
    if (csPin != GPIO_PIN_NONE) {
#ifdef SPI4_CS_ACTIVE_HIGH
        GPIO_PinSet(csPin);
#else
        GPIO_PinClear(csPin);
#endif
    }
    DCH5INTbits.CHBCIF = 0; // clear RX DMA channel interrupt flag
    DCH5CONbits.CHEN = 1; // enable RX DMA channel
    DCH4CONbits.CHEN = 1; // enable TX DMA channel to begin transfer
}

/**
 * @brief DMA interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Dma5InterruptHandler(void) {
    EVIC_SourceStatusClear(INT_SOURCE_DMA5); // clear interrupt flag first because transfer complete callback may start a new transfer
    if (csPin != GPIO_PIN_NONE) {
#ifdef SPI4_CS_ACTIVE_HIGH
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
bool Spi4DmaTransferInProgress(void) {
    return DCH5CONbits.CHEN == 1; // if RX DMA channel interrupt enabled
}

//------------------------------------------------------------------------------
// End of file
