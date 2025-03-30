/**
 * @file Commands.c
 * @author Seb Madgwick
 * @brief Commands.
 */

//------------------------------------------------------------------------------
// Includes

#include "Commands.h"

//------------------------------------------------------------------------------
// Functions

/**
 * @brief Ping command.
 * @param value Value.
 * @param response Response.
 * @param context Context.
 */
void CommandsPing(const char* * const value, Ximu3CommandResponse * const response, void* const context) {
    if (Ximu3CommandParseNull(value, response) != 0) {
        return;
    }
    Ximu3CommandRespondPing(response, "Twintig", "01234567"); // TODO: use context for MUX
}

//------------------------------------------------------------------------------
// End of file
