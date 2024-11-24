/**
*   @file    Gpio.h
*   @brief   Declaration of function prototypes and special parameters.
*   @details This file contains the definitions for structures, the Gpio_Type,
*          		the base address of GPIO, and measures to prevent multiple declarations.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef SCG_H
#define SCG_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "clock_registers.h"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void set_system_clock_source(unsigned int source);
void SOSC_init_8MHz(void);
void SPLL_init_160MHz(void);
void NormalRUNmode_80MHz (void);

#endif
