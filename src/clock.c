/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "clock.h"

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*!
 * @brief  System clock source constants.
 *
 * @details Defines constants for different system clock sources to ensure clarity and
 *          avoid using hardcoded values in the code.
 */
#define SYSTEM_CLOCK_SOURCE_MAIN_OSC  0
#define SYSTEM_CLOCK_SOURCE_PLL       1
#define SYSTEM_CLOCK_SOURCE_SIRC      2
#define SYSTEM_CLOCK_SOURCE_FIRC      3

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*!
 * @brief Configures the System Oscillator (SOSC) as the system clock source.
 *
 * @details This function selects the System Oscillator (SOSC) as the clock source for the system.
 *          It modifies the Run Clock Control Register (RCCR) to set SOSC as the current clock source.
 */
void set_SOSC_source(){
    SCG->RCCR_bits.RCCR_SCS = 1;
}

/*!
 * @brief Configures the System PLL (SPLL) as the system clock source.
 *
 * @details This function selects the System PLL (SPLL) as the clock source for both the
 *          Run Clock Control Register (RCCR) and High Speed Run Clock Control Register (HCCR).
 */
void set_SPLL_source(){
    SCG->RCCR_bits.RCCR_SCS = 6;
    SCG->HCCR_bits.HCCR_SCS = 6;
}

/*!
 * @brief Configures the Slow IRC (SIRC) as the system clock source.
 *
 * @details This function selects the Slow IRC (SIRC) as the clock source for both the
 *          Run Clock Control Register (RCCR) and Very Low Power Clock Control Register (VCCR).
 */
void set_SIRC_source(){
    SCG->RCCR_bits.RCCR_SCS = 2;
    SCG->VCCR_bits.VCCR_SCS = 2;
}

/*!
 * @brief Configures the Fast IRC (FIRC) as the system clock source.
 *
 * @details This function selects the Fast IRC (FIRC) as the clock source for both the
 *          Run Clock Control Register (RCCR) and High Speed Run Clock Control Register (HCCR).
 */
void set_FIRC_source(){
    SCG->RCCR_bits.RCCR_SCS = 3;
    SCG->HCCR_bits.HCCR_SCS = 3;
}

/*!
 * @brief Sets the system clock source based on the specified input.
 *
 * @details This function allows the selection of the system clock source based on the input parameter.
 *          It uses a switch case to select between SOSC, SPLL, SIRC, and FIRC clock sources.
 *
 * @param[in] source: The desired system clock source (0: SOSC, 1: SPLL, 2: SIRC, 3: FIRC).
 */
void set_system_clock_source(unsigned int source){
    switch(source){
        case SYSTEM_CLOCK_SOURCE_MAIN_OSC:
            set_SOSC_source();
            break;
        case SYSTEM_CLOCK_SOURCE_PLL:
            set_SPLL_source();
            break;
        case SYSTEM_CLOCK_SOURCE_SIRC:
            set_SIRC_source();
            break;
        case SYSTEM_CLOCK_SOURCE_FIRC:
            set_FIRC_source();
            break;
        default:
            set_SOSC_source();  /* Default to SOSC if invalid source is provided */
            break;
    }
}

/*!
 * @brief Initializes the System Oscillator (SOSC) with an 8 MHz external crystal.
 *
 * @details This function configures and enables the System Oscillator (SOSC) to operate with an
 *          8 MHz external crystal. It sets the clock range, enables the oscillator, and waits
 *          until the oscillator is valid.
 */
void SOSC_init_8MHz(void)
{
    /*!
     * SOSC Initialization (8 MHz):
     * ===================================================
     */
    SCG->SOSCDIV_bits.SOSCDIV_SOSCDIV1 = 1;
    SCG->SOSCDIV_bits.SOSCDIV_SOSCDIV2 = 1;  

    SCG->SOSCCFG_bits.SOSCCFG_RANGE = 2;
    /* Range=2: Medium frequency (SOSC between 1MHz-8MHz) */
    SCG->SOSCCFG_bits.SOSCCFG_ERREFS = 1; /* Use external XTAL as input */

    while(SCG->SOSCCSR_bits.SOSCCSR_LK); /* Ensure SOSCCSR is unlocked */
    SCG->SOSCCSR_bits.SOSCCSR_SOSCEN = 1; /* Enable the oscillator */

    while(!(SCG->SOSCCSR_bits.SOSCCSR_SOSCVLD)); /* Wait for SOSC to be valid */
}

/*!
 * @brief Initializes the System PLL (SPLL) to generate a 160 MHz clock.
 *
 * @details This function configures and enables the System PLL (SPLL) to generate a 160 MHz clock
 *          based on an 8 MHz SOSC input. It sets the divider and multiplier values, enables the PLL,
 *          and waits until the PLL output is valid.
 */
void SPLL_init_160MHz(void)
{
    /*!
     * SPLL Initialization (160 MHz):
     * ===================================================
     */
    while(SCG->SPLLCSR_bits.SPLLCSR_LK); /* Ensure SPLLCSR is unlocked */
    SCG->SPLLCSR_bits.SPLLCSR_SPLLEN = 0; /* Disable SPLL before configuration */

    SCG->SPLLDIV_bits.SPLLDIV_SPLLDIV1 = 2; /* Set SPLLDIV1 to divide by 2 */
    SCG->SPLLDIV_bits.SPLLDIV_SPLLDIV2 = 3; /* Set SPLLDIV2 to divide by 4 */

    SCG->SPLLCFG_bits.SPLLCFG_MULT = 24; /* Multiply SOSC by 40 (8 MHz * 40 = 320 MHz) */
    /* SPLL_CLK = 8 MHz / 1 * 40 / 2 = 160 MHz */

    while(SCG->SPLLCSR_bits.SPLLCSR_LK); /* Ensure SPLLCSR is unlocked */
    SCG->SPLLCSR_bits.SPLLCSR_SPLLEN = 1; /* Enable the SPLL */

    while(!(SCG->SPLLCSR_bits.SPLLCSR_SPLLVLD)); /* Wait for SPLL to be valid */
}

/*!
 * @brief Configures the system to operate in Normal RUN mode at 80 MHz.
 *
 * @details This function sets up the system to run at 80 MHz using the PLL clock derived from
 *          an 8 MHz SOSC input. It configures the Slow IRC (SIRC) for asynchronous clock sources,
 *          then switches the system clock source to the SPLL.
 */
void NormalRUNmode_80MHz (void)
{
    /*!
     * SIRC Initialization:
     * ===================================================
     * Slow IRC is enabled with high range (8 MHz) in reset.
     * Enable SIRCDIV1_CLK and SIRCDIV2_CLK, divide by 1 = 8 MHz asynchronous clock source.
     */
    SCG->SIRCDIV_bits.SIRCDIV_SIRCDIV1 = 1;
    SCG->SIRCDIV_bits.SIRCDIV_SIRCDIV2 = 1;

    /*!
     * System Clock Configuration (80 MHz):
     * ===================================================
     * Change to Normal RUN mode with 8 MHz SOSC, 160 MHz PLL divided by 2:
     */
    SCG->SCG_RCCR = 0x06010012U;
    /* DIVCORE=1, div. by 2: Core clock = 160 MHz / 2 = 80 MHz */
    /* Select PLL as clock source */

    while (((SCG->SCG_CSR) >> 24 ) != 6) {} /* Wait for system clock source to switch to SPLL */
}
