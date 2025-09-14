/**
 * @file Icm8.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM8_H
#define ICM8_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm8.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm8;

//------------------------------------------------------------------------------
// Function declaration

void Icm8Initialise(const IcmSettings * const settings);
void Icm8Deinitialise(void);
IcmResult Icm8GetData(IcmData * const data);
uint32_t Icm8BufferOverflow(void);
IcmTestResult Icm8Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
