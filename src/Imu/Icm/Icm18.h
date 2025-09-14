/**
 * @file Icm18.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM18_H
#define ICM18_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm18.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm18;

//------------------------------------------------------------------------------
// Function declaration

void Icm18Initialise(const IcmSettings * const settings);
void Icm18Deinitialise(void);
IcmResult Icm18GetData(IcmData * const data);
uint32_t Icm18BufferOverflow(void);
IcmTestResult Icm18Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
