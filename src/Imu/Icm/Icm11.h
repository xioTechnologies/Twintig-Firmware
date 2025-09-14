/**
 * @file Icm11.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM11_H
#define ICM11_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm11.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm11;

//------------------------------------------------------------------------------
// Function declaration

void Icm11Initialise(const IcmSettings * const settings);
void Icm11Deinitialise(void);
IcmResult Icm11GetData(IcmData * const data);
uint32_t Icm11BufferOverflow(void);
IcmTestResult Icm11Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
