/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "systick.h"

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

/*!
 * @brief Enables the SysTick timer.
 *
 * This function sets the enable bit in the SysTick Control and Status Register (SYST_CSR),
 * starting the SysTick timer.
 */
void SysTick_Enable(){
    SYSTICK->SYST_CSR.ENABLE = 1;
}

/*!
 * @brief Initializes the SysTick timer with the specified configuration.
 *
 * This function configures the SysTick timer using the provided configuration structure.
 * It sets the clock source, interrupt mode, and enables the SysTick timer.
 *
 * @param[in] config: Configuration structure containing the desired settings.
 */
void SysTick_Init(systick_config_t config){
    SYSTICK->SYST_CSR.CLK_SOURCE = config.clk_source;
    SYSTICK->SYST_CSR.TICKINT = config.interrupt_mode;
    SYSTICK->SYST_CSR.ENABLE = 1;
}

/*!
 * @brief Disables the SysTick timer.
 *
 * This function clears the enable bit in the SysTick Control and Status Register (SYST_CSR),
 * stopping the SysTick timer.
 */
void SysTick_Disable(){
    SYSTICK->SYST_CSR.ENABLE = 0;
}

/*!
 * @brief Delays execution for a specified number of milliseconds.
 *
 * This function uses the SysTick timer to create a delay for a given number of milliseconds.
 * The delay is based on the provided core clock frequency.
 *
 * @param[in] ms:         Number of milliseconds to delay.
 * @param[in] core_clock: The core clock frequency in Hz. Assumed to be 48 MHz.
 */
void delay(unsigned int ms, unsigned int core_clock){
    systick_config_t config_ms;
    config_ms.clk_source = 1;            /* Use core clock as SysTick clock source */
    config_ms.interrupt_mode = 0;        /* Disable SysTick interrupts */
    
    /* Set the reload value to generate 1 ms delay */
    SYSTICK->SYST_RVR.RELOAD = core_clock / 1000 - 1;
    
    /* Initialize SysTick timer with the configuration */
    SysTick_Init(config_ms);
    
    /* Wait for the specified number of milliseconds */
    for(unsigned int counter = 0; counter < ms; counter++) {
        while(!(SYSTICK->SYST_CSR.COUNTFLAG));  /* Wait until the COUNTFLAG is set */
    }
    
    /* Disable SysTick timer after delay */
    SysTick_Disable();
}

/*!
 * @brief Sets the reload value for the SysTick timer.
 *
 * This function sets the reload value in the SysTick Reload Value Register (SYST_RVR),
 * determining the count value that the SysTick timer will use before resetting.
 *
 * @param[in] reload: Reload value to be set.
 */
void SysTick_SetReload(unsigned int reload){
    SYSTICK->SYST_RVR.RELOAD = reload;
}
