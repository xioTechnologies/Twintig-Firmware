/**
 * @file Icm19.h
 * @author Seb Madgwick
 * @brief ICM-42688-P driver.
 */

#ifndef ICM19_H
#define ICM19_H

//------------------------------------------------------------------------------
// Includes

#include "Icm.h"
#include "Icm19.h"
#include <stdint.h>

//------------------------------------------------------------------------------
// Variable declarations

extern const Icm icm19;

//------------------------------------------------------------------------------
// Function declaration

void Icm19Initialise(const IcmSettings * const settings);
void Icm19Deinitialise(void);
IcmResult Icm19GetData(IcmData * const data);
uint32_t Icm19BufferOverflow(void);
IcmTestResult Icm19Test(void);

#endif

//------------------------------------------------------------------------------
// End of file
