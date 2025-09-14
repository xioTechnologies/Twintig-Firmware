/**
 * @file Icm9.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM9_H
#define ICM9_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm9.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm9;

//------------------------------------------------------------------------------
// Function declaration

void Icm9Initialise(const IcmSettings * const settings);
void Icm9Deinitialise(void);
IcmResult Icm9GetData(IcmData * const data);
uint32_t Icm9BufferOverflow(void);
IcmTestResult Icm9Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
