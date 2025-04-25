/**
 * @file Icm1.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM1_H
#define ICM1_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm1.h"

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm1;

//------------------------------------------------------------------------------
// Function declaration

void Icm1Initialise(const IcmOdr odr);
void Icm1Deinitialise(void);
IcmResult Icm1GetData(IcmData * const data);
uint32_t Icm1BufferOverflow(void);
IcmTestResult Icm1Test(void);

//------------------------------------------------------------------------------
// End of file

#endif
