/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "RTC.h"

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#define SIM_LPOCLKS (*(unsigned int*)(0x40048000u + 0x10))

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
 * @brief     Initializes the RTC module.
 *
 * @details   This function configures the RTC module, including selecting the 32kHz LPO clock
 *            for the RTC, resetting various registers, and enabling the RTC module.
 */
void RTC_init(void)
{
    unsigned int sim_lpoclks_mask = 0;

    /* Select 32kHz LPO clock for RTC_CLK */
    sim_lpoclks_mask = SIM_LPOCLKS;
    sim_lpoclks_mask |= (1 << 4);
    sim_lpoclks_mask |= (1 << 1);
    SIM_LPOCLKS = sim_lpoclks_mask;  /* Write to LPO clock select register */

    /* RTC Initialization */
    RTC->SR &= ~(1 << 4);     /* Disable RTC module */
    RTC->TPR = 0;             /* Reset Time Prescaler Register */
    RTC->TSR = 0;             /* Start count from 0 */

    /* Configure RTC */
    RTC->TCR = 0;             /* Set time compensation to 0 */
    RTC->CR |= (0 << 7);      /* Use RTC_CLK instead of LPO */
    RTC->CR |= (1 << 5);      /* Select TSIC output for RTC_CLKOUT */

    RTC->IER &= ~(1 << 16);   /* Disable 1Hz output */
    RTC->IER &= ~(1 << 17);   /* Disable 1Hz output */
    RTC->IER &= ~(1 << 18);   /* Disable 1Hz output */
    RTC->IER |= (1 << 4);     /* Enable alarm interrupt */

    /* Enable RTC module */
    RTC->SR |= (1 << 4);
}
