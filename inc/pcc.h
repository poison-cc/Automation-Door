/**
*   @file    pcc.h
*   @brief   Declaration of function prototypes and special parameters for the PCC module.
*   @details This file contains the definitions for structures, the PCC_Type,
*            the base address of PCC, and measures to prevent multiple declarations.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef PCC_H_
#define PCC_H_

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "pcc_registers.h"

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void PCC_EnableClock(volatile unsigned int *pccRegister);
void PCC_DisableClock(volatile unsigned int *pccRegister);
void PCC_SetClockSource(volatile unsigned int *pccRegister, unsigned int clockSource);
void PCC_SetClockFraction(volatile unsigned int *pccRegister, unsigned int clockFraction);
void PCC_SelectClockDivider(volatile unsigned int *pccRegister, unsigned int clockDivider);
unsigned int PCC_GetClockStatus(volatile unsigned int *pccRegister);
unsigned int PCC_IsPeripheralPresent(volatile unsigned int *pccRegister);

#endif // PCC_H_

/*==================================================================================================
==================================================================================================*/
