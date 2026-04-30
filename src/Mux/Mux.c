/**
 * @file Mux.c
 * @author Seb Madgwick
 * @brief Multiplexer.
 */

//------------------------------------------------------------------------------
// Includes

#include "Mux.h"
#include "Serial/Serial.h"
#include "Usb/UsbCdc.h"
#include "Ximu3Device/x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Interface.
 */
typedef struct {
    size_t(*const availableWrite)(void);
    FifoResult(*const write)(const void* const data, const size_t numberOfBytes);
} Interface;

//------------------------------------------------------------------------------
// Function declarations

static inline __attribute__((always_inline)) size_t AvailableWrite(const Interface * const interface, const MuxChannel channel);
static inline __attribute__((always_inline)) FifoResult Write(const Interface * const interface, const MuxChannel channel, const void* const data, const size_t numberOfBytes);

//------------------------------------------------------------------------------
// Variables

static const Interface usb = {.availableWrite = UsbCdcAvailableWrite, .write = UsbCdcWrite};
static const Interface serial = {.availableWrite = SerialAvailableWrite, .write = SerialWrite};

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Returns the byte value of the mux channel.
 * @param channel Channel.
 * @return Byte value of the mux channel.
 */
uint8_t MuxChannelToByte(const MuxChannel channel) {
    switch (channel) {
        case MuxChannelNone:
            break;
        case MuxChannelA:
            return 'A';
        case MuxChannelB:
            return 'B';
        case MuxChannelC:
            return 'C';
        case MuxChannelD:
            return 'D';
        case MuxChannelE:
            return 'E';
        case MuxChannelF:
            return 'F';
        case MuxChannelG:
            return 'G';
        case MuxChannelH:
            return 'H';
        case MuxChannelI:
            return 'I';
        case MuxChannelJ:
            return 'J';
        case MuxChannelK:
            return 'K';
        case MuxChannelL:
            return 'L';
        case MuxChannelM:
            return 'M';
        case MuxChannelN:
            return 'N';
        case MuxChannelO:
            return 'O';
        case MuxChannelP:
            return 'P';
        case MuxChannelQ:
            return 'Q';
        case MuxChannelR:
            return 'R';
        case MuxChannelS:
            return 'S';
        case MuxChannelT:
            return 'T';
    }
    return '\n'; // avoid compiler warning
}

/**
 * @brief Returns the space available in the write buffer.
 * @param channel Channel.
 * @return Space available in the write buffer.
 */
size_t MuxUsbAvailableWrite(const MuxChannel channel) {
    return AvailableWrite(&usb, channel);
}

/**
 * @brief Writes data to the write buffer.
 * @param channel Channel.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
FifoResult MuxUsbWrite(const MuxChannel channel, const void* const data, const size_t numberOfBytes) {
    return Write(&usb, channel, data, numberOfBytes);
}

/**
 * @brief Returns the space available in the write buffer.
 * @param channel Channel.
 * @return Space available in the write buffer.
 */
size_t MuxSerialAvailableWrite(const MuxChannel channel) {
    return AvailableWrite(&serial, channel);
}

/**
 * @brief Writes data to the write buffer.
 * @param channel Channel.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
FifoResult MuxSerialWrite(const MuxChannel channel, const void* const data, const size_t numberOfBytes) {
    return Write(&serial, channel, data, numberOfBytes);
}

/**
 * @brief Returns the space available in the write buffer.
 * @param interface Interface.
 * @param channel Channel.
 * @return Space available in the write buffer.
 */
static inline __attribute__((always_inline)) size_t AvailableWrite(const Interface * const interface, const MuxChannel channel) {
    if (channel == MuxChannelNone) {
        return interface->availableWrite();
    }
    const size_t available = interface->availableWrite();
    if (available < XIMU3_SIZE_MUX_HEADER) {
        return 0;
    }
    return available - XIMU3_SIZE_MUX_HEADER;
}

/**
 * @brief Writes data to the write buffer.
 * @param interface Interface.
 * @param channel Channel.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
static inline __attribute__((always_inline)) FifoResult Write(const Interface * const interface, const MuxChannel channel, const void* const data, const size_t numberOfBytes) {
    if (channel == MuxChannelNone) {
        return interface->write(data, numberOfBytes);
    }
    const uint8_t header[] = {'^', MuxChannelToByte(channel)};
    if (interface->availableWrite() < (sizeof (header) + numberOfBytes)) {
        return FifoResultError;
    }
    interface->write(header, sizeof (header));
    interface->write(data, numberOfBytes);
    return FifoResultOk;
}

//------------------------------------------------------------------------------
// End of file
