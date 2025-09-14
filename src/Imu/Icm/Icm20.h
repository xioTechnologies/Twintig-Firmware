/**
 * @file Icm20.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM20_H
#define ICM20_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm20.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm20;

//------------------------------------------------------------------------------
// Function declaration

void Icm20Initialise(const IcmSettings * const settings);
void Icm20Deinitialise(void);
IcmResult Icm20GetData(IcmData * const data);
uint32_t Icm20BufferOverflow(void);
IcmTestResult Icm20Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
