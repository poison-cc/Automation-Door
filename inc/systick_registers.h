/**
*   @file    systick_registers.h
*   @brief   Register definitions for the systick module.
*   @details Defines the structure for systick registers, base address, and includes guards 
*            to prevent multiple declarations.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef SYSTICK_REGISTER_H
#define SYSTICK_REGISTER_H

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          systick structure.
* @details        Structure defining the RTC registers and their offsets.
*/
typedef struct {
  union {
    volatile const unsigned int SYST_CSR_Register;  /* Offset: 0x00 */
    struct {
        volatile unsigned int ENABLE      : 1;  /* Enable SysTick counter */
        volatile unsigned int TICKINT     : 1;  /* Enable SysTick exception request */
        volatile unsigned int CLK_SOURCE  : 1;  /* Clock source selection */
        volatile const unsigned int RESERVED1 : 13; /* Reserved bits */
        volatile unsigned int COUNTFLAG   : 1;  /* Returns 1 if timer counted to 0 */
        volatile unsigned int RESERVED2   : 15; /* Reserved bits */
    } SYST_CSR;
  };
  union {
    volatile const unsigned int SYST_RVR_Register;  /* Offset: 0x04 */
    struct {
        volatile unsigned int RELOAD      : 24; /* Reload value for the SysTick timer */
        volatile const unsigned int RESERVED1 : 8; /* Reserved bits */
    } SYST_RVR;
  };
  union {
    volatile const unsigned int SYST_CVR_Register;  /* Offset: 0x08 */
    struct {
        volatile unsigned int CURRENT     : 24; /* Current value of the SysTick timer */
        volatile const unsigned int RESERVED1 : 8; /* Reserved bits */
    } SYST_CVR;
  };
  volatile const unsigned int SYST_CALIB;  /* Offset: 0x0C - Calibration value */
}systick_type_t;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define SYSTICK_BASE           (0xE000E010U) /* Base address for SysTick registers */
/** Pointer to the SysTick base address */
#define SYSTICK                ((systick_type_t*)(SYSTICK_BASE))

#endif /* SYSTICK_REGISTER_H */
