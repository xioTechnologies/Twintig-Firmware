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
    size_t(*availableWrite)(void);
    FifoResult(*write)(const void* const data, const size_t numberOfBytes);
} Interface;

//------------------------------------------------------------------------------
// Function declarations

static inline __attribute__((always_inline)) size_t AvailableWrite(const Interface * const interface, const MuxChannel channel);
static inline __attribute__((always_inline)) size_t Write(const Interface * const interface, const MuxChannel channel, const void* const data, const size_t numberOfBytes);

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
        case MuxChannel1:
            return 'A';
        case MuxChannel2:
            return 'B';
        case MuxChannel3:
            return 'C';
        case MuxChannel4:
            return 'D';
        case MuxChannel5:
            return 'E';
        case MuxChannel6:
            return 'F';
        case MuxChannel7:
            return 'G';
        case MuxChannel8:
            return 'H';
        case MuxChannel9:
            return 'I';
        case MuxChannel10:
            return 'J';
        case MuxChannel11:
            return 'K';
        case MuxChannel12:
            return 'L';
        case MuxChannel13:
            return 'M';
        case MuxChannel14:
            return 'N';
        case MuxChannel15:
            return 'O';
        case MuxChannel16:
            return 'P';
        case MuxChannel17:
            return 'Q';
        case MuxChannel18:
            return 'R';
        case MuxChannel19:
            return 'S';
        case MuxChannel20:
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
    if (available < XIMU3_MUX_HEADER_SIZE) {
        return 0;
    }
    return available - XIMU3_MUX_HEADER_SIZE;
}

/**
 * @brief Writes data to the write buffer.
 * @param interface Interface.
 * @param channel Channel.
 * @param data Data.
 * @param numberOfBytes Number of bytes.
 * @return Result.
 */
static inline __attribute__((always_inline)) size_t Write(const Interface * const interface, const MuxChannel channel, const void* const data, const size_t numberOfBytes) {
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
