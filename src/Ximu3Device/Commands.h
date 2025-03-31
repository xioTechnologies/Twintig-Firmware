/**
 * @file Commands.h
 * @author Seb Madgwick
 * @brief Commands.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

//------------------------------------------------------------------------------
// Includes

#include <stdbool.h>
#include "x-IMU3-Device/Ximu3Command.h"

//------------------------------------------------------------------------------
// Function declarations

void CommandsDefault(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsSave(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsPing(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsHaptic(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsFactory(const char* * const value, Ximu3CommandResponse * const response, void* const context);
bool CommandsOverrideReadOnly(void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
