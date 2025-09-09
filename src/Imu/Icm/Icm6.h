/**
 * @file Icm6.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM6_H
#define ICM6_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm6.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm6;

//------------------------------------------------------------------------------
// Function declaration

void Icm6Initialise(const IcmSettings * const settings);
void Icm6Deinitialise(void);
IcmResult Icm6GetData(IcmData * const data);
uint32_t Icm6BufferOverflow(void);
IcmTestResult Icm6Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
