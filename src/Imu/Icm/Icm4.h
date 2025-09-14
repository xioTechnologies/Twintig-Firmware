/**
 * @file Icm4.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM4_H
#define ICM4_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm4.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm4;

//------------------------------------------------------------------------------
// Function declaration

void Icm4Initialise(const IcmSettings * const settings);
void Icm4Deinitialise(void);
IcmResult Icm4GetData(IcmData * const data);
uint32_t Icm4BufferOverflow(void);
IcmTestResult Icm4Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
