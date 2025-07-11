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
    const size_t(*availableWrite)(void);
    const FifoResult(*write)(const void* const data, const size_t numberOfBytes);
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
    switch (channel) {
        case MuxChannelNone:
            return interface->write(data, numberOfBytes);
        case MuxChannel1:
        {
            if (numberOfBytes > interface->availableWrite()) {
                return FifoResultError;
            }
            interface->write("^A", XIMU3_MUX_HEADER_SIZE);
            interface->write(data, numberOfBytes);
            return FifoResultOk;
        }
        case MuxChannel2:
        {
            if (numberOfBytes > interface->availableWrite()) {
                return FifoResultError;
            }
            interface->write("^B", XIMU3_MUX_HEADER_SIZE);
            interface->write(data, numberOfBytes);
            return FifoResultOk;
        }
        case MuxChannel3:
        {
            if (numberOfBytes > interface->availableWrite()) {
                return FifoResultError;
            }
            interface->write("^C", XIMU3_MUX_HEADER_SIZE);
            interface->write(data, numberOfBytes);
            return FifoResultOk;
        }
        case MuxChannel4:
        {
            if (numberOfBytes > interface->availableWrite()) {
                return FifoResultError;
            }
            interface->write("^D", XIMU3_MUX_HEADER_SIZE);
            interface->write(data, numberOfBytes);
            return FifoResultOk;
        }
        case MuxChannel5:
        {
            if (numberOfBytes > interface->availableWrite()) {
                return FifoResultError;
            }
            interface->write("^E", XIMU3_MUX_HEADER_SIZE);
            interface->write(data, numberOfBytes);
            return FifoResultOk;
        }
    }
    return FifoResultError; // avoid compiler warning
}

//------------------------------------------------------------------------------
// End of file
