/**
 * @file Icm10.h
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
 */

#ifndef ICM10_H
#define ICM10_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm10;

//------------------------------------------------------------------------------
// Function declarations

void Icm10Initialise(const IcmSettings * const settings);
void Icm10Deinitialise(void);
IcmResult Icm10GetData(IcmData * const data);
uint32_t Icm10BufferOverflow(void);
IcmTestResult Icm10Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
