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

//------------------------------------------------------------------------------
// Definitions

/**
 * @brief Mux channel.
 */
typedef enum {
    MuxChannelNone,
    MuxChannel1,
    MuxChannel2,
    MuxChannel3,
    MuxChannel4,
    MuxChannel5,
} MuxChannel;

//------------------------------------------------------------------------------
// Function declarations

size_t MuxUsbAvailableWrite(const MuxChannel channel);
FifoResult MuxUsbWrite(const MuxChannel channel, const void* const data, const size_t numberOfBytes);
size_t MuxSerialAvailableWrite(const MuxChannel channel);
FifoResult MuxSerialWrite(const MuxChannel channel, const void* const data, const size_t numberOfBytes);

#endif

//------------------------------------------------------------------------------
// End of file
