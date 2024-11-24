/**
*   @file    Nvic.h
*   @brief   Declearation of function prototype and enumeration of IRQn_Type
*   @details Function prototype and enumeration with Each IRQ number corresponds to a specific 
* 					 interrupt source in the microcontroller.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef NVIC_H
#define NVIC_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "nvic_registers.h"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
 * @brief Enumeration of Interrupt Request Numbers (IRQn_Type) for NVIC.
 *
 * @detail This enumeration defines IRQ numbers corresponding to specific interrupts in the NVIC 
 * Each IRQ number corresponds to a specific interrupt source in the microcontroller.
 */
typedef enum {
    IRQ_DMA0 = 0,                              /* DMA channel 0 transfer complete */
    IRQ_DMA1 = 1,                              /* DMA channel 1 transfer complete */
    IRQ_DMA2 = 2,                              /* DMA channel 2 transfer complete */
    IRQ_DMA3 = 3,                             /* DMA channel 3 transfer complete */
    IRQ_DMA4 = 4,                              /* DMA channel 4 transfer complete */
    IRQ_DMA5 = 5,                              /* DMA channel 5 transfer complete */
    IRQ_DMA6 = 6,                              /* DMA channel 6 transfer complete */
    IRQ_DMA7 = 7,                              /* DMA channel 7 transfer complete */
    IRQ_DMA8 = 8,                              /* DMA channel 8 transfer complete */
    IRQ_DMA9 = 9,                              /* DMA channel 9 transfer complete */
    IRQ_DMA10 = 10,                             /* DMA channel 10 transfer complete */
    IRQ_DMA11 = 11,                             /* DMA channel 11 transfer complete */
    IRQ_DMA12 = 12,                             /* DMA channel 12 transfer complete */
    IRQ_DMA13 = 13,                             /* DMA channel 13 transfer complete */
    IRQ_DMA14 = 14,                             /* DMA channel 14 transfer complete */
    IRQ_DMA15 = 15,                             /* DMA channel 15 transfer complete */
    IRQ_DMA_ERROR = 16,                         /* DMA error interrupt channels 0-15 */
    IRQ_MCM = 17,                               /* FPU sources */
    IRQ_FTFC = 18,                              /* FTFC Command complete */
    IRQ_READ_COLLISION = 19,                    /* FTFC Read collision */
    IRQ_LVD_LVW = 20,                           /* PMC Low voltage detect interrupt */
    IRQ_FTFC_FAULT = 21,                        /* FTFC Double bit fault detect */
    IRQ_WDOG_EWM = 22,                          /* Single interrupt vector for WDOG and EWM */
    IRQ_RCM = 23,                               /* RCM Asynchronous Interrupt */
    IRQ_LPI2C0_MASTER = 24,                     /* LPI2C0 Master Interrupt */
    IRQ_LPI2C0_SLAVE = 25,                      /* LPI2C0 Slave Interrupt */
    IRQ_LPSPI0 = 26,                            /* LPSPI0 Interrupt */
    IRQ_LPSPI1 = 27,                            /* LPSPI1 Interrupt */
    IRQ_LPSPI2 = 28,                            /* LPSPI2 Interrupt */
    IRQ_RESERVED45 = 29,                        /* Reserved Interrupt 45 */
    IRQ_RESERVED46 = 30,                        /* Reserved Interrupt 46 */
    IRQ_LPUART0_RXTX = 31,                      /* LPUART0 Transmit / Receive Interrupt */
    IRQ_RESERVED48 = 32,                        /* Reserved Interrupt 48 */
    IRQ_LPUART1_RXTX = 33,                      /* LPUART1 Transmit / Receive Interrupt */
    IRQ_RESERVED50 = 34,                        /* Reserved Interrupt 50 */
    IRQ_LPUART2_RXTX = 35,                      /* LPUART2 Transmit / Receive Interrupt */
    IRQ_RESERVED52 = 36,                        /* Reserved Interrupt 52 */
    IRQ_RESERVED53 = 37,                        /* Reserved Interrupt 53 */
    IRQ_RESERVED54 = 38,                        /* Reserved Interrupt 54 */
    IRQ_ADC0 = 39,                              /* ADC0 interrupt request */
    IRQ_ADC1 = 40,                              /* ADC1 interrupt request */
    IRQ_CMP0 = 41,                              /* CMP0 interrupt request */
    IRQ_RESERVED58 = 42,                        /* Reserved Interrupt 58 */
    IRQ_RESERVED59 = 43,                        /* Reserved Interrupt 59 */
    IRQ_ERM_SINGLE_FAULT = 44,                  /* ERM single bit error correction */
    IRQ_ERM_DOUBLE_FAULT = 45,                  /* ERM double bit error non-correctable */
    IRQ_RTC = 46,                               /* RTC alarm interrupt */
    IRQ_RTC_SECONDS = 47,                       /* RTC seconds interrupt */
    IRQ_LPIT0_CH0 = 48,                         /* LPIT0 channel 0 overflow interrupt */
    IRQ_LPIT0_CH1 = 49,                         /* LPIT0 channel 1 overflow interrupt */
    IRQ_LPIT0_CH2 = 50,                         /* LPIT0 channel 2 overflow interrupt */
    IRQ_LPIT0_CH3 = 51,                         /* LPIT0 channel 3 overflow interrupt */
    IRQ_PDB0 = 52,                              /* PDB0 interrupt */
    IRQ_RESERVED69 = 53,                        /* Reserved Interrupt 69 */
    IRQ_RESERVED70 = 54,                        /* Reserved Interrupt 70 */
    IRQ_RESERVED71 = 55,                        /* Reserved Interrupt 71 */
    IRQ_RESERVED72 = 56,                        /* Reserved Interrupt 72 */
    IRQ_SCG = 57,                               /* SCG bus interrupt request */
    IRQ_LPTMR0 = 58,                            /* LPTIMER interrupt request */
    IRQ_PORTA = 59,                             /* Port A pin detect interrupt */
    IRQ_PORTB = 60,                             /* Port B pin detect interrupt */
    IRQ_PORTC = 61,                             /* Port C pin detect interrupt */
    IRQ_PORTD = 62,                             /* Port D pin detect interrupt */
    IRQ_PORTE = 63,                             /* Port E pin detect interrupt */
    IRQ_SWI = 64,                               /* Software interrupt */
    IRQ_RESERVED81 = 65,                        /* Reserved Interrupt 81 */
    IRQ_RESERVED82 = 66,                        /* Reserved Interrupt 82 */
    IRQ_RESERVED83 = 67,                        /* Reserved Interrupt 83 */
    IRQ_PDB1 = 68,                              /* PDB1 interrupt */
    IRQ_FLEXIO = 69,                            /* FlexIO Interrupt */
    IRQ_RESERVED86 = 70,                        /* Reserved Interrupt 86 */
    IRQ_RESERVED87 = 71,                        /* Reserved Interrupt 87 */
    IRQ_RESERVED88 = 72,                        /* Reserved Interrupt 88 */
    IRQ_RESERVED89 = 73,                        /* Reserved Interrupt 89 */
    IRQ_RESERVED90 = 74,                        /* Reserved Interrupt 90 */
    IRQ_RESERVED91 = 75,                        /* Reserved Interrupt 91 */
    IRQ_RESERVED92 = 76,                        /* Reserved Interrupt 92 */
    IRQ_RESERVED93 = 77,                        /* Reserved Interrupt 93 */
    IRQ_CAN0_ORED = 78,                         /* CAN0 OR'ed [Bus Off OR Transmit Warning OR Receive Warning] */
    IRQ_CAN0_ERROR = 79,                        /* CAN0 Interrupt indicating that errors were detected on the CAN bus */
    IRQ_CAN0_WAKE_UP = 80,                      /* CAN0 Interrupt asserted when Pretended Networking operation is enabled, and a valid message matches the selected filter criteria during Low Power mode */
    IRQ_CAN0_ORED_0_15_MB = 81,                 /* CAN0 OR'ed Message buffer (0-15) */
    IRQ_CAN0_ORED_16_31_MB = 82,                /* CAN0 OR'ed Message buffer (16-31) */
    IRQ_RESERVED99 = 83,                        /* Reserved Interrupt 99 */
    IRQ_RESERVED100 = 84,                       /* Reserved Interrupt 100 */
    IRQ_CAN1_ORED = 85,                         /* CAN1 OR'ed [Bus Off OR Transmit Warning OR Receive Warning] */
    IRQ_CAN1_ERROR = 86,                        /* CAN1 Interrupt indicating that errors were detected on the CAN bus */
    IRQ_RESERVED103 = 87,                       /* Reserved Interrupt 103 */
    IRQ_CAN1_ORED_0_15_MB = 88,                 /* CAN1 OR'ed Interrupt for Message buffer (0-15) */
    IRQ_RESERVED105 = 89,                       /* Reserved Interrupt 105 */
    IRQ_RESERVED106 = 90,                       /* Reserved Interrupt 106 */
    IRQ_RESERVED107 = 91,                       /* Reserved Interrupt 107 */
    IRQ_CAN2_ORED = 92,                         /* CAN2 OR'ed [Bus Off OR Transmit Warning OR Receive Warning] */
    IRQ_CAN2_ERROR = 93,                        /* CAN2 Interrupt indicating that errors were detected on the CAN bus */
    IRQ_RESERVED110 = 94,                       /* Reserved Interrupt 110 */
    IRQ_CAN2_ORED_0_15_MB = 95,                 /* CAN2 OR'ed Message buffer (0-15) */
    IRQ_RESERVED112 = 96,                       /* Reserved Interrupt 112 */
    IRQ_RESERVED113 = 97,                       /* Reserved Interrupt 113 */
    IRQ_RESERVED114 = 98,                       /* Reserved Interrupt 114 */
    IRQ_FTM0_CH0_CH1 = 99,                      /* FTM0 Channel 0 and 1 interrupt */
    IRQ_FTM0_CH2_CH3 = 100,                      /* FTM0 Channel 2 and 3 interrupt */
    IRQ_FTM0_CH4_CH5 = 101,                      /* FTM0 Channel 4 and 5 interrupt */
    IRQ_FTM0_CH6_CH7 = 102,                      /* FTM0 Channel 6 and 7 interrupt */
    IRQ_FTM0_FAULT = 103,                        /* FTM0 Fault interrupt */
    IRQ_FTM0_OVF_RELOAD = 104,                   /* FTM0 Counter overflow and Reload interrupt */
    IRQ_FTM1_CH0_CH1 = 105,                      /* FTM1 Channel 0 and 1 interrupt */
    IRQ_FTM1_CH2_CH3 = 106,                      /* FTM1 Channel 2 and 3 interrupt */
    IRQ_FTM1_CH4_CH5 = 107,                      /* FTM1 Channel 4 and 5 interrupt */
    IRQ_FTM1_CH6_CH7 = 108,                      /* FTM1 Channel 6 and 7 interrupt */
    IRQ_FTM1_FAULT = 109,                      /* FTM1 Fault interrupt */
    IRQ_FTM1_OVF_RELOAD = 110,                   /* FTM1 Counter overflow and Reload interrupt */
    IRQ_FTM2_CH0_CH1 = 111,                      /* FTM2 Channel 0 and 1 interrupt */
    IRQ_FTM2_CH2_CH3 = 112,                      /* FTM2 Channel 2 and 3 interrupt */
} IRQn_Type;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void NVIC_EnableIRQ(unsigned int IRQn);
void NVIC_DisableIRQ(unsigned int IRQn);
void NVIC_SetPendingIRQ(unsigned int IRQn);
void NVIC_ClearPendingIRQ(unsigned int IRQn);
unsigned int NVIC_GetActive(unsigned int IRQn);
void NVIC_SetPriority(unsigned int IRQn, unsigned int priority);
unsigned int NVIC_GetPriority(unsigned int IRQn);
void NVIC_TriggerIRQ(unsigned int IRQn);

#endif
