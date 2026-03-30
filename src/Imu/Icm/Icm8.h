/**
 * @file Icm8.h
 * @author Seb Madgwick
 * @brief ICM-45686 driver.
 */

#ifndef ICM8_H
#define ICM8_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm8;

//------------------------------------------------------------------------------
// Function declarations

void Icm8Initialise(const IcmSettings * const settings);
void Icm8Deinitialise(void);
IcmResult Icm8GetData(IcmData * const data);
uint32_t Icm8BufferOverflow(void);
IcmTestResult Icm8Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
