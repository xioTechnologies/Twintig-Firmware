/**
 * @file Icm16.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM16_H
#define ICM16_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm16;

//------------------------------------------------------------------------------
// Function declarations

void Icm16Initialise(const IcmSettings * const settings);
void Icm16Deinitialise(void);
IcmResult Icm16GetData(IcmData * const data);
uint32_t Icm16BufferOverflow(void);
IcmTestResult Icm16Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
