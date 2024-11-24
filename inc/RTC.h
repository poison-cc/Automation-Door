/**
*   @file    RTC.h
*   @brief   Declaration of function prototypes and special parameters for the RTC module.
*   @details This file contains the declarations for the RTC module, including function prototypes,
*
*/

/*==================================================================================================
==================================================================================================*/

#ifndef RTC_H
#define RTC_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "rtc_registers.h"

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief Initializes the RTC module.
 * 
 * This function configures the RTC module with the required settings,
 * enabling time tracking and setting up necessary control registers.
 */
void RTC_init(void);

#endif /* RTC_H */
