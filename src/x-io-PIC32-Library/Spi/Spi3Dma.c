/**
 * @file Spi3Dma.c
 * @author Seb Madgwick
 * @brief SPI driver using DMA for PIC32 devices.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
#include "definitions.h"
#include "Spi3Dma.h"
#include "sys/kmem.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Uncomment this line to print transfers.
 */
//#define PRINT_TRANSFERS

//------------------------------------------------------------------------------
// Variables

const Spi spi3Dma = {
    .transfer = Spi3DmaTransfer,
    .transferInProgress = Spi3DmaTransferInProgress,
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
void Spi3DmaInitialise(const SpiSettings * const settings) {

    // Ensure default register states
    Spi3DmaDeinitialise();

    // Configure SPI
    SPI3CONbits.MSTEN = 1; // host mode
    SPI3CONbits.ENHBUF = 1; // enhanced Buffer mode is enabled
    SPI3CONbits.SMP = 1; // input data sampled at end of data output time
    SPI3CONbits.CKP = settings->clockPolarity;
    SPI3CONbits.CKE = settings->clockPhase;
    SPI3CONbits.STXISEL = 0b11; // interrupt is generated when the buffer is not full (has one or more empty elements)
    SPI3CONbits.SRXISEL = 0b01; // interrupt is generated when the buffer is not empty
    SPI3BRG = SpiCalculateSpixbrg(settings->clockFrequency);
    SPI3CONbits.ON = 1;

    // Enable DMA
    DMACONbits.ON = 1;

    // Configure TX DMA channel
#ifdef _SPI3_TX_IRQ
    DCH2ECONbits.CHSIRQ = _SPI3_TX_IRQ;
#else
    DCH2ECONbits.CHSIRQ = _SPI3_TX_VECTOR;
#endif
    DCH2ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH2DSA = KVA_TO_PA(&SPI3BUF); // destination address
    DCH2DSIZ = 1; // destination size
    DCH2CSIZ = 1; // transfers per event

    // Configure RX DMA channel
#ifdef _SPI3_RX_IRQ
    DCH3ECONbits.CHSIRQ = _SPI3_RX_IRQ;
#else
    DCH3ECONbits.CHSIRQ = _SPI3_RX_VECTOR;
#endif
    DCH3ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH3SSA = KVA_TO_PA(&SPI3BUF); // source address
    DCH3SSIZ = 1; // source size
    DCH3CSIZ = 1; // transfers per event
    DCH3INTbits.CHBCIE = 1; // channel Block Transfer Complete Interrupt Enable bit

    // Configure RX DMA channel interrupt
    EVIC_SourceEnable(INT_SOURCE_DMA3);
}

/**
 * @brief Deinitialises the module.
 */
void Spi3DmaDeinitialise(void) {

    // Disable SPI and restore default register states
    SPI3CON = 0;
    SPI3CON2 = 0;
    SPI3STAT = 0;
    SPI3BRG = 0;

    // Disable TX DMA channel and restore default register states
    DCH2CON = 0;
    DCH2ECON = 0;
    DCH2INT = 0;
    DCH2SSA = 0;
    DCH2DSA = 0;
    DCH2SSIZ = 0;
    DCH2DSIZ = 0;
    DCH2SPTR = 0;
    DCH2DPTR = 0;
    DCH2CSIZ = 0;
    DCH2CPTR = 0;
    DCH2DAT = 0;

    // Disable RX DMA channel and restore default register states
    DCH3CON = 0;
    DCH3ECON = 0;
    DCH3INT = 0;
    DCH3SSA = 0;
    DCH3DSA = 0;
    DCH3SSIZ = 0;
    DCH3DSIZ = 0;
    DCH3SPTR = 0;
    DCH3DPTR = 0;
    DCH3CSIZ = 0;
    DCH3CPTR = 0;
    DCH3DAT = 0;

    // Disable interrupt
    EVIC_SourceDisable(INT_SOURCE_DMA3);
    EVIC_SourceStatusClear(INT_SOURCE_DMA3);
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
void Spi3DmaTransfer(const GPIO_PIN csPin_, volatile void* const data_, const size_t numberOfBytes_, void (*const transferComplete_) (void)) {

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
    DCH2SSA = KVA_TO_PA(data_); // source address
    DCH2SSIZ = numberOfBytes_; // source size

    // Configure RX DMA channel
    DCH3DSA = KVA_TO_PA(data_); // destination address
    DCH3DSIZ = numberOfBytes_; // destination size

    // Begin transfer
    if (csPin != GPIO_PIN_NONE) {
#ifdef SPI3_CS_ACTIVE_HIGH
        GPIO_PinSet(csPin);
#else
        GPIO_PinClear(csPin);
#endif
    }
    DCH3INTbits.CHBCIF = 0; // clear RX DMA channel interrupt flag
    DCH3CONbits.CHEN = 1; // enable RX DMA channel
    DCH2CONbits.CHEN = 1; // enable TX DMA channel to begin transfer
}

/**
 * @brief DMA interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Dma3InterruptHandler(void) {
    EVIC_SourceStatusClear(INT_SOURCE_DMA3); // clear interrupt flag first because transfer complete callback may start new transfer
    if (csPin != GPIO_PIN_NONE) {
#ifdef SPI3_CS_ACTIVE_HIGH
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
bool Spi3DmaTransferInProgress(void) {
    return DCH3CONbits.CHEN == 1; // if RX DMA channel interrupt enabled
}

//------------------------------------------------------------------------------
// End of file
