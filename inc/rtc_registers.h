/**
*   @file    rtc_registers.h
*   @brief   Definition of Register for RTC module
*   @details Structure, Rtc_Type, Rtc_Base address, security to avoid multiple declarations
*/

/*==================================================================================================
==================================================================================================*/

#ifndef RTC_REGISTER_H
#define RTC_REGISTER_H

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          struct name RTC_Type
* @details        This structure is for RTC Register and its offset
*
*/
/** RTC - Register Layout Typedef */

typedef struct {
  volatile unsigned int TSR;   /**< RTC Time Seconds Register, offset: 0x0 */
  volatile unsigned int TPR;   /**< RTC Time Prescaler Register, offset: 0x4 */
  volatile unsigned int TAR;   /**< RTC Time Alarm Register, offset: 0x8 */
  volatile unsigned int TCR;   /**< RTC Time Compensation Register, offset: 0xC */
  volatile unsigned int CR;    /**< RTC Control Register, offset: 0x10 */
  volatile unsigned int SR;    /**< RTC Status Register, offset: 0x14 */
  volatile unsigned int LR;    /**< RTC Lock Register, offset: 0x18 */
  volatile unsigned int IER;   /**< RTC Interrupt Enable Register, offset: 0x1C */
} RTC_Type;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#define RTC_BASE    (0x4003D000U)
/** Peripheral RTC base pointer */
#define RTC         ((RTC_Type *)RTC_BASE)

#endif /* RTC_REGISTER_H */
