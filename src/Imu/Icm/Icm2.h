/**
 * @file Icm2.h
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
 */

#ifndef ICM2_H
#define ICM2_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm2;

//------------------------------------------------------------------------------
// Function declarations

void Icm2Initialise(const IcmSettings * const settings);
void Icm2Deinitialise(void);
IcmResult Icm2GetData(IcmData * const data);
uint32_t Icm2BufferOverflow(void);
IcmTestResult Icm2Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
