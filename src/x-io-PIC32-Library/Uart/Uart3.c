/**
 * @file Uart3.c
 * @author Seb Madgwick
 * @brief UART driver for PIC32 devices.
 */

//------------------------------------------------------------------------------
// Includes

#include "Config.h"
#include "definitions.h"
#include <stdint.h>
#include "Uart3.h"

//------------------------------------------------------------------------------
// Function declarations

static inline __attribute__((always_inline)) void RXInterruptTasks(void);
static inline __attribute__((always_inline)) void TXInterruptTasks(void);

//------------------------------------------------------------------------------
// Variables

static bool receiveBufferOverrun;
static uint8_t readData[UART3_READ_BUFFER_SIZE];
static Fifo readFifo = {.data = readData, .dataSize = sizeof (readData)};
static uint8_t writeData[UART3_WRITE_BUFFER_SIZE];
static Fifo writeFifo = {.data = writeData, .dataSize = sizeof (writeData)};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Initialises the module.
 * @param settings Settings.
 */
void Uart3Initialise(const UartSettings * const settings) {

    // Ensure default register states
    Uart3Deinitialise();

    // Configure UART
    if (settings->rtsCtsEnabled) {
        U3MODEbits.UEN = 0b10; // UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
    }
    if (settings->invertTXRX) {
        U3MODEbits.RXINV = 1; // UxRX Idle state is '0'
        U3STAbits.UTXINV = 1; // UxTX Idle state is '0'
    }
    U3MODEbits.PDSEL = settings->parityAndData;
    U3MODEbits.STSEL = settings->stopBits;
    U3MODEbits.BRGH = 1; // high-Speed mode - 4x baud clock enabled
    U3STAbits.URXISEL = 0b01; // interrupt flag bit is asserted while receive buffer is 1/2 or more full (i.e., has 4 or more data characters)
    U3STAbits.UTXISEL = 0b10; // interrupt is generated and asserted while the transmit buffer is empty
    U3STAbits.URXEN = 1; // UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
    U3STAbits.UTXEN = 1; // UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
    U3BRG = UartCalculateUxbrg(settings->baudRate);
    U3MODEbits.ON = 1; // UARTx is enabled. UARTx pins are controlled by UARTx as defined by UEN<1:0> and UTXEN control bits

    // Enable interrupts
    EVIC_SourceEnable(INT_SOURCE_UART3_RX);
}

/**
 * @brief Deinitialises the module.
 */
void Uart3Deinitialise(void) {

    // Disable UART and restore default register states
    U3MODE = 0;
    U3STA = 0;

    // Disable interrupts
    EVIC_SourceDisable(INT_SOURCE_UART3_RX);
    EVIC_SourceDisable(INT_SOURCE_UART3_TX);
    EVIC_SourceStatusClear(INT_SOURCE_UART3_RX);
    EVIC_SourceStatusClear(INT_SOURCE_UART3_TX);

    // Clear buffers
    Uart3ClearReadBuffer();
    Uart3ClearWriteBuffer();
}

/**
 * @brief Returns the number of bytes available in the read buffer.
 * @return Number of bytes available in the read buffer.
 */
size_t Uart3AvailableRead(void) {

    // Trigger RX interrupt if hardware receive buffer not empty
    if (U3STAbits.URXDA == 1) {
        EVIC_SourceEnable(INT_SOURCE_UART3_RX);
        EVIC_SourceStatusSet(INT_SOURCE_UART3_RX);
    }

    // Clear hardware receive buffer overrun flag
    if (U3STAbits.OERR == 1) {
        U3STAbits.OERR = 0;
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
size_t Uart3Read(void* const destination, size_t numberOfBytes) {
    Uart3AvailableRead(); // process hardware receive buffer
    return FifoRead(&readFifo, destination, numberOfBytes);
}

/**
 * @brief Reads a byte from the read buffer. This function must only be called
 * if there are bytes available in the read buffer.
 * @return Byte.
 */
uint8_t Uart3ReadByte(void) {
    return FifoReadByte(&readFifo);
}

/**
 * @brief Returns the space available in the write buffer.
 * @return Space available in the write buffer.
 */
size_t Uart3AvailableWrite(void) {
    return FifoAvailableWrite(&writeFifo);
}

/**
 * @brief Writes data to the write buffer.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
FifoResult Uart3Write(const void* const data, const size_t numberOfBytes) {
    const FifoResult result = FifoWrite(&writeFifo, data, numberOfBytes);
    EVIC_SourceEnable(INT_SOURCE_UART3_TX);
    return result;
}

/**
 * @brief Writes a byte to the write buffer.
 * @param byte Byte.
 * @return Result.
 */
FifoResult Uart3WriteByte(const uint8_t byte) {
    const FifoResult result = FifoWriteByte(&writeFifo, byte);
    EVIC_SourceEnable(INT_SOURCE_UART3_TX);
    return result;
}

/**
 * @brief Clears the read buffer and resets the read buffer overrun flag.
 */
void Uart3ClearReadBuffer(void) {
    FifoClear(&readFifo);
    Uart3ReceiveBufferOverrun();
}

/**
 * @brief Clears the write buffer.
 */
void Uart3ClearWriteBuffer(void) {
    FifoClear(&writeFifo);
}

/**
 * @brief Returns true if the hardware receive buffer has overrun. Calling this
 * function will reset the flag.
 * @return True if the hardware receive buffer has overrun.
 */
bool Uart3ReceiveBufferOverrun(void) {
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
bool Uart3TransmitionComplete(void) {
    return (EVIC_SourceIsEnabled(INT_SOURCE_UART3_TX) == false) && (U3STAbits.TRMT == 1);
}

#ifdef _UART_3_VECTOR

/**
 * @brief UART RX and TX interrupt handler. This function should be called by
 * the ISR implementation generated by MPLAB Harmony.
 */
void Uart3InterruptHandler(void) {

    // RX interrupt
    if (EVIC_SourceStatusGet(INT_SOURCE_UART3_RX)) {
        RXInterruptTasks();
    }

    // TX interrupt
    if (EVIC_SourceIsEnabled(INT_SOURCE_UART3_TX) == false) {
        return; // return if TX interrupt disabled because TX interrupt flag will remain set while the transmit buffer is empty
    }
    if (EVIC_SourceStatusGet(INT_SOURCE_UART3_TX)) {
        TXInterruptTasks();
    }
}

#else

/**
 * @brief UART RX interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Uart3RXInterruptHandler(void) {
    RXInterruptTasks();
}

/**
 * @brief UART TX interrupt handler. This function should be called by the ISR
 * implementation generated by MPLAB Harmony.
 */
void Uart3TXInterruptHandler(void) {
    TXInterruptTasks();
}

#endif

/**
 * @brief UART RX interrupt tasks.
 */
static inline __attribute__((always_inline)) void RXInterruptTasks(void) {
    while (U3STAbits.URXDA == 1) { // while data available in receive buffer
        if (FifoAvailableWrite(&readFifo) == 0) { // if read buffer full
            EVIC_SourceDisable(INT_SOURCE_UART3_RX);
            break;
        } else {
            FifoWriteByte(&readFifo, U3RXREG);
        }
    }
    EVIC_SourceStatusClear(INT_SOURCE_UART3_RX);
}

/**
 * @brief UART TX interrupt tasks.
 */
static inline __attribute__((always_inline)) void TXInterruptTasks(void) {
    EVIC_SourceDisable(INT_SOURCE_UART3_TX); // disable TX interrupt to avoid nested interrupt
    EVIC_SourceStatusClear(INT_SOURCE_UART3_TX);
    while (U3STAbits.UTXBF == 0) { // while transmit buffer not full
        if (FifoAvailableRead(&writeFifo) == 0) { // if write buffer empty
            return;
        }
        U3TXREG = FifoReadByte(&writeFifo);
    }
    EVIC_SourceEnable(INT_SOURCE_UART3_TX); // re-enable TX interrupt
}

//------------------------------------------------------------------------------
// End of file
