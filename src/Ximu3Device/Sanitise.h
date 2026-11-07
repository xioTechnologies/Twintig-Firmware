/**
 * @file Sanitise.h
 * @author Seb Madgwick
 * @brief Sanitise settings.
 */

#ifndef SANITISE_H
#define SANITISE_H

//------------------------------------------------------------------------------
// Includes

#include "x-IMU3-Device/Ximu3.h"

//------------------------------------------------------------------------------
// Function declarations

void Sanitise(const Ximu3SettingsIndex index, void* const context);

#endif

//------------------------------------------------------------------------------
// End of file
