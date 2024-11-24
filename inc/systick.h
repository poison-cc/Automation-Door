/**
*   @file    systick.h
*   @brief   Header file for the SysTick module.
*   @details This file contains the declarations for the SysTick module, including the function 
*            prototypes, structure definitions, and macros necessary for configuring and using 
*            the SysTick timer.
*/
#ifndef SYSTICK_H_
#define SYSTICK_H_

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "systick_registers.h"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*!
 * @brief Configuration structure for the SysTick timer.
 *
 * This structure is used to configure the SysTick timer with the specified clock source 
 * and interrupt mode options.
 *
 * @param[in] clk_source:     Clock source for the SysTick timer (0 = external, 1 = core clock)
 * @param[in] interrupt_mode: Interrupt mode enable/disable (0 = disabled, 1 = enabled)
 */
typedef struct {
    unsigned char clk_source     : 1;
    unsigned char interrupt_mode : 1;
}systick_config_t;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void SysTick_Enable();
void SysTick_Init(systick_config_t config);
void SysTick_Disable();
void delay(unsigned int ms, unsigned int core_clock);
void SysTick_SetReload(unsigned int reload);

#endif /* SYSTICK_H_ */
