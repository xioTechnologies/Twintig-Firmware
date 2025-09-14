/**
 * @file Icm15.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM15_H
#define ICM15_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm15.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm15;

//------------------------------------------------------------------------------
// Function declaration

void Icm15Initialise(const IcmSettings * const settings);
void Icm15Deinitialise(void);
IcmResult Icm15GetData(IcmData * const data);
uint32_t Icm15BufferOverflow(void);
IcmTestResult Icm15Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
