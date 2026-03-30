/**
 * @file Icm5.h
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
 */

#ifndef ICM5_H
#define ICM5_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm5;

//------------------------------------------------------------------------------
// Function declarations

void Icm5Initialise(const IcmSettings * const settings);
void Icm5Deinitialise(void);
IcmResult Icm5GetData(IcmData * const data);
uint32_t Icm5BufferOverflow(void);
IcmTestResult Icm5Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
