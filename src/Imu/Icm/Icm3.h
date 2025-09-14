/**
 * @file Icm3.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM3_H
#define ICM3_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm3.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm3;

//------------------------------------------------------------------------------
// Function declaration

void Icm3Initialise(const IcmSettings * const settings);
void Icm3Deinitialise(void);
IcmResult Icm3GetData(IcmData * const data);
uint32_t Icm3BufferOverflow(void);
IcmTestResult Icm3Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
