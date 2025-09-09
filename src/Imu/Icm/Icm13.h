/**
 * @file Icm13.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM13_H
#define ICM13_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm13.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm13;

//------------------------------------------------------------------------------
// Function declaration

void Icm13Initialise(const IcmSettings * const settings);
void Icm13Deinitialise(void);
IcmResult Icm13GetData(IcmData * const data);
uint32_t Icm13BufferOverflow(void);
IcmTestResult Icm13Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
