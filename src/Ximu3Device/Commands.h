/**
 * @file Commands.h
 * @author Seb Madgwick
 * @brief Commands.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

//------------------------------------------------------------------------------
// Includes

#include "x-IMU3-Device/Ximu3Command.h"

//------------------------------------------------------------------------------
// Function declarations

void CommandsPing(const char* * const value, Ximu3CommandResponse * const response, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
