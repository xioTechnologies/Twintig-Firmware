/**
 * @file Icm14.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM14_H
#define ICM14_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm14;

//------------------------------------------------------------------------------
// Function declarations

void Icm14Initialise(const IcmSettings * const settings);
void Icm14Deinitialise(void);
IcmResult Icm14GetData(IcmData * const data);
uint32_t Icm14BufferOverflow(void);
IcmTestResult Icm14Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
