/**
 * @file Icm1.h
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
 */

#ifndef ICM1_H
#define ICM1_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm1;

//------------------------------------------------------------------------------
// Function declarations

void Icm1Initialise(const IcmSettings * const settings);
void Icm1Deinitialise(void);
IcmResult Icm1GetData(IcmData * const data);
uint32_t Icm1BufferOverflow(void);
IcmTestResult Icm1Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
