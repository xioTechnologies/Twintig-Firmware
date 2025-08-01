/**
 * @file Uart1DmaTx.c
 * @author Seb Madgwick
 * @brief UART driver using DMA for PIC32 devices. DMA used for TX only.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
#include "definitions.h"
#include "Fifo.h"
#include <stdint.h>
#include "sys/kmem.h"
#include "Uart1DmaTx.h"

//------------------------------------------------------------------------------
// Function declarations

static inline __attribute__((always_inline)) void RXInterruptTasks(void);

//------------------------------------------------------------------------------
// Variables

static bool receiveBufferOverrun;
static uint8_t readData[UART1_READ_BUFFER_SIZE];
static Fifo readFifo = {.data = readData, .dataSize = sizeof (readData)};
static void (*writeComplete)(void);

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module.
 * @param settings Settings.
 */
void Uart1DmaTxInitialise(const UartSettings * const settings) {

    // Ensure default register states
    Uart1DmaTxDeinitialise();

    // Configure UART
    if (settings->rtsCtsEnabled) {
        U1MODEbits.UEN = 0b10; // UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
    }
    if (settings->invertTXRX) {
        U1MODEbits.RXINV = 1; // UxRX Idle state is '0'
        U1STAbits.UTXINV = 1; // UxTX Idle state is '0'
    }
    U1MODEbits.PDSEL = settings->parityAndData;
    U1MODEbits.STSEL = settings->stopBits;
    U1MODEbits.BRGH = 1; // high-Speed mode - 4x baud clock enabled
    U1STAbits.URXISEL = 0b01; // interrupt flag bit is asserted while receive buffer is 1/2 or more full (i.e., has 4 or more data characters)
    U1STAbits.URXEN = 1; // UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
    U1STAbits.UTXEN = 1; // UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
    U1BRG = UartCalculateUxbrg(settings->baudRate);
    U1MODEbits.ON = 1; // UARTx is enabled. UARTx pins are controlled by UARTx as defined by UEN<1:0> and UTXEN control bits

    // Enable DMA
    DMACONbits.ON = 1;

    // Configure TX DMA channel
    DCH1ECONbits.CHSIRQ = _UART1_TX_VECTOR;
    DCH1ECONbits.SIRQEN = 1; // start channel cell transfer if an interrupt matching CHSIRQ occurs
    DCH1DSA = KVA_TO_PA(&U1TXREG); // destination address
    DCH1DSIZ = 1; // destination size
    DCH1CSIZ = 1; // transfers per event
    DCH1INTbits.CHBCIE = 1; // channel Block Transfer Complete Interrupt Enable bit

    // Enable interrupts
    EVIC_SourceEnable(INT_SOURCE_UART1_RX);
    EVIC_SourceEnable(INT_SOURCE_DMA1);
}

/**
 * @brief Deinitialises the module.
 */
void Uart1DmaTxDeinitialise(void) {

    // Disable UART and restore default register states
    U1MODE = 0;
    U1STA = 0;

    // Disable TX DMA channel and restore default register states
    DCH1CON = 0;
    DCH1ECON = 0;
    DCH1INT = 0;
    DCH1SSA = 0;
    DCH1DSA = 0;
    DCH1SSIZ = 0;
    DCH1DSIZ = 0;
    DCH1SPTR = 0;
    DCH1DPTR = 0;
    DCH1CSIZ = 0;
    DCH1CPTR = 0;
    DCH1DAT = 0;

    // Disable interrupts
    EVIC_SourceDisable(INT_SOURCE_UART1_RX);
    EVIC_SourceDisable(INT_SOURCE_DMA1);
    EVIC_SourceStatusClear(INT_SOURCE_UART1_RX);
    EVIC_SourceStatusClear(INT_SOURCE_DMA1);

    // Clear buffer
    Uart1DmaTxClearReadBuffer();
}

/**
 * @brief Returns the number of bytes available in the read buffer.
 * @return Number of bytes available in the read buffer.
 */
size_t Uart1DmaTxAvailableRead(void) {

    // Trigger RX interrupt if hardware receive buffer not empty
    if (U1STAbits.URXDA == 1) {
        EVIC_SourceEnable(INT_SOURCE_UART1_RX);
        EVIC_SourceStatusSet(INT_SOURCE_UART1_RX);
    }

    // Clear hardware receive buffer overrun flag
    if (U1STAbits.OERR == 1) {
        U1STAbits.OERR = 0;
        receiveBufferOverrun = true;
    }

    // Return number of bytes
    return FifoAvailableRead(&readFifo);
}

/**
 * @brief Reads data from the read buffer.
 * @param destination Destination.
 * @param numberOfBytes Number of bytes.
 * @return Number of bytes read.
 */
size_t Uart1DmaTxRead(void* const destination, size_t numberOfBytes) {
    Uart1DmaTxAvailableRead(); // process hardware receive buffer
    return FifoRead(&readFifo, destination, numberOfBytes);
}

/**
 * @brief Reads a byte from the read buffer. This function must only be called
 * if there are bytes available in the read buffer.
 * @return Byte.
 */
uint8_t Uart1DmaTxReadByte(void) {
    return FifoReadByte(&readFifo);
}

/**
 * @brief Writes data. The data must be declared __attribute__((coherent)) for
 * PIC32MZ devices. This function must not be called while a write is in
 * progress.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @param writeComplete_ Write complete callback.
 */
void Uart1DmaTxWrite(const void* const data, const size_t numberOfBytes, void (*const writeComplete_) (void)) {
    writeComplete = writeComplete_;
    DCH1SSA = KVA_TO_PA(data); // source address
    DCH1SSIZ = numberOfBytes; // source size
    DCH1INTbits.CHBCIF = 0; // clear TX DMA channel interrupt flag
    DCH1CONbits.CHEN = 1; // enable TX DMA channel to begin write
}

/**
 * @brief DMA interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Dma1InterruptHandler(void) {
    EVIC_SourceStatusClear(INT_SOURCE_DMA1); // clear interrupt flag first because callback may start new write
    if (writeComplete != NULL) {
        writeComplete();
    }
}

/**
 * @brief Returns true while data is being transferred to the hardware transmit
 * buffer.
 * @return True while data is being transferred to the hardware transmit buffer.
 */
bool Uart1DmaTxWriteInProgress(void) {
    return DCH1CONbits.CHEN == 1;
}

/**
 * @brief Clears the read buffer and resets the read buffer overrun flag.
 */
void Uart1DmaTxClearReadBuffer(void) {
    FifoClear(&readFifo);
    Uart1DmaTxReceiveBufferOverrun();
}

/**
 * @brief Returns true if the hardware receive buffer has overrun. Calling this
 * function will reset the flag.
 * @return True if the hardware receive buffer has overrun.
 */
bool Uart1DmaTxReceiveBufferOverrun(void) {
    if (receiveBufferOverrun) {
        receiveBufferOverrun = false;
        return true;
    }
    return false;
}

/**
 * @brief Returns true if all data has been transmitted.
 * @return True if all data has been transmitted.
 */
bool Uart1DmaTxTransmitionComplete(void) {
    return (Uart1DmaTxWriteInProgress() == false) && (U1STAbits.TRMT == 1);
}

#ifdef _UART_1_VECTOR

/**
 * @brief UART RX and TX interrupt handler. This function should be called by
 * the ISR implementation generated by MPLAB Harmony.
 */
void Uart1DmaTxInterruptHandler(void) {
    if (EVIC_SourceStatusGet(INT_SOURCE_UART1_RX)) {
        RXInterruptTasks();
    }
}

#else

/**
 * @brief UART RX interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Uart1RXInterruptHandler(void) {
    RXInterruptTasks();
}

#endif

/**
 * @brief UART RX interrupt tasks.
 */
static inline __attribute__((always_inline)) void RXInterruptTasks(void) {
    while (U1STAbits.URXDA == 1) { // while data available in receive buffer
        if (FifoAvailableWrite(&readFifo) == 0) { // if read buffer full
            EVIC_SourceDisable(INT_SOURCE_UART1_RX);
            break;
        } else {
            FifoWriteByte(&readFifo, U1RXREG);
        }
    }
    EVIC_SourceStatusClear(INT_SOURCE_UART1_RX);
}

//------------------------------------------------------------------------------
// End of file
