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
    MuxChannelA,
    MuxChannelB,
    MuxChannelC,
    MuxChannelD,
    MuxChannelE,
    MuxChannelF,
    MuxChannelG,
    MuxChannelH,
    MuxChannelI,
    MuxChannelJ,
    MuxChannelK,
    MuxChannelL,
    MuxChannelM,
    MuxChannelN,
    MuxChannelO,
    MuxChannelP,
    MuxChannelQ,
    MuxChannelR,
    MuxChannelS,
    MuxChannelT,
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
