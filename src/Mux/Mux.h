/**
 * @file Mux.h
 * @author Seb Madgwick
 * @brief Multiplexer.
 */

#ifndef MUX_H
#define MUX_H

//------------------------------------------------------------------------------
// Includes

#include "Fifo.h"
#include <stddef.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Channel.
 */
typedef enum {
    MuxChannelNone,
    MuxChannel1,
    MuxChannel2,
    MuxChannel3,
    MuxChannel4,
    MuxChannel5,
    MuxChannel6,
    MuxChannel7,
    MuxChannel8,
    MuxChannel9,
    MuxChannel10,
    MuxChannel11,
    MuxChannel12,
    MuxChannel13,
    MuxChannel14,
    MuxChannel15,
    MuxChannel16,
    MuxChannel17,
    MuxChannel18,
    MuxChannel19,
    MuxChannel20,
} MuxChannel;

//------------------------------------------------------------------------------
// Function declarations

uint8_t MuxChannelToByte(const MuxChannel channel);
size_t MuxUsbAvailableWrite(const MuxChannel channel);
FifoResult MuxUsbWrite(const MuxChannel channel, const void* const data, const size_t numberOfBytes);
size_t MuxSerialAvailableWrite(const MuxChannel channel);
FifoResult MuxSerialWrite(const MuxChannel channel, const void* const data, const size_t numberOfBytes);

#endif

//------------------------------------------------------------------------------
// End of file
