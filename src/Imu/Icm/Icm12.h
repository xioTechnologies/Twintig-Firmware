/**
 * @file Icm12.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM12_H
#define ICM12_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm12.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm12;

//------------------------------------------------------------------------------
// Function declaration

void Icm12Initialise(const IcmSettings * const settings);
void Icm12Deinitialise(void);
IcmResult Icm12GetData(IcmData * const data);
uint32_t Icm12BufferOverflow(void);
IcmTestResult Icm12Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
