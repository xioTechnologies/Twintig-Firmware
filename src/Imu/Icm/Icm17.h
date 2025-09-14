/**
 * @file Icm17.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM17_H
#define ICM17_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm17.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm17;

//------------------------------------------------------------------------------
// Function declaration

void Icm17Initialise(const IcmSettings * const settings);
void Icm17Deinitialise(void);
IcmResult Icm17GetData(IcmData * const data);
uint32_t Icm17BufferOverflow(void);
IcmTestResult Icm17Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
