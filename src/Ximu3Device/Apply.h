/**
 * @file Apply.h
 * @author Seb Madgwick
 * @brief Apply.
 */

#ifndef APPLY_H
#define APPLY_H

//------------------------------------------------------------------------------
// Includes

#include "Context.h"

//------------------------------------------------------------------------------
// Function declarations

void ApplyTasks(Context * const context);
void ApplyNow(Context * const context);
void ApplyAfterDelay(Context * const context);

#endif

//------------------------------------------------------------------------------
// End of file
