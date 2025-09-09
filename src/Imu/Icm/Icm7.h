/**
 * @file Icm7.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM7_H
#define ICM7_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm7.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm7;

//------------------------------------------------------------------------------
// Function declaration

void Icm7Initialise(const IcmSettings * const settings);
void Icm7Deinitialise(void);
IcmResult Icm7GetData(IcmData * const data);
uint32_t Icm7BufferOverflow(void);
IcmTestResult Icm7Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
