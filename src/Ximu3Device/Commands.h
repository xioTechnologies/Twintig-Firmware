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
#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Function declarations

void CommandsDefault(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsApply(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsSave(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsPing(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsBlink(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsStrobe(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsColour(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsHaptic(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsInitialise(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsHeading(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsNote(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsTimestamp(const char* * const value, Ximu3CommandResponse * const response, void* const context);
void CommandsFactory(const char* * const value, Ximu3CommandResponse * const response, void* const context);
bool CommandsOverrideReadOnly(void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
