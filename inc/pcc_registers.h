/**
*   @file    pcc_registers.h
*   @brief   Definition of Registers for the PCC (Peripheral Clock Control) module
*   @details This file contains the structure definition for the PCC module, including
*            register definitions, the base address for the PCC module, and a type definition
*            for the PCC registers. It ensures that the structure is only declared once.
*/

/*==================================================================================================
    @note     The PCC module provides clock control for various peripherals in the microcontroller.
              This header file defines the structure and base address for accessing the PCC registers.
==================================================================================================*/

#ifndef PCC_REGISTER_H_
#define PCC_REGISTER_H_

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          struct name PCC_Type
* @details        This structure represents the PCC registers and their offsets
*
*/

typedef struct {
    unsigned int RESERVED_0[32];       // Offset 0x00 to 0x7C (Reserved)
    volatile unsigned int PCC_FTFC;    // Offset 0x80
    volatile unsigned int PCC_DMAMUX;  // Offset 0x84
    unsigned int RESERVED_1[2];        // Offset 0x88 and 0x8C
    volatile unsigned int PCC_FlexCAN0;// Offset 0x90
    volatile unsigned int PCC_FlexCAN1;// Offset 0x94
    volatile unsigned int PCC_FTM3;    // Offset 0x98
    volatile unsigned int PCC_ADC1;    // Offset 0x9C
    unsigned int RESERVED_2[3];        // Offset 0xA0 to 0xA8 (Reserved)
    volatile unsigned int PCC_FlexCAN2;// Offset 0xAC
    volatile unsigned int PCC_LPSPI0;  // Offset 0xB0
    volatile unsigned int PCC_LPSPI1;  // Offset 0xB4
    volatile unsigned int PCC_LPSPI2;  // Offset 0xB8
    unsigned int RESERVED_3[2];        // Offset 0xBC and 0xC0 (Reserved)
    volatile unsigned int PCC_PDB1;    // Offset 0xC4
    volatile unsigned int PCC_CRC;     // Offset 0xC8
    unsigned int RESERVED_4[3];        // Offset 0xCC and 0xD4 and 0xD4(Reserved)
    volatile unsigned int PCC_PDB0;    // Offset 0xD8
    volatile unsigned int PCC_LPIT;    // Offset 0xDC
    volatile unsigned int PCC_FTM0;    // Offset 0xE0
    volatile unsigned int PCC_FTM1;    // Offset 0xE4
    volatile unsigned int PCC_FTM2;    // Offset 0xE8
    volatile unsigned int PCC_ADC0;    // Offset 0xEC
    unsigned int RESERVED_5[1];        // Offset 0xF0 (Reserved)
    volatile unsigned int PCC_RTC;     // Offset 0xF4
    unsigned int RESERVED_6[2];        // Offset 0xF8 to 0xFC (Reserved)
    volatile unsigned int PCC_LPTMR0;  // Offset 0x100
    unsigned int RESERVED_7[8];        // Offset 0x104 to 0x120 (Reserved)
    volatile unsigned int PCC_PORTA;   // Offset 0x124
    volatile unsigned int PCC_PORTB;   // Offset 0x128
    volatile unsigned int PCC_PORTC;   // Offset 0x12C
    volatile unsigned int PCC_PORTD;   // Offset 0x130
    volatile unsigned int PCC_PORTE;   // Offset 0x134
    unsigned int RESERVED_8[6]     ;   // Offset 0x138 to 0x14C (Reserved)
    volatile unsigned int PCC_SAI0;    // Offset 0x150
    volatile unsigned int PCC_SAI1;    // Offset 0x154
    unsigned int RESERVED_9[4];        // Offset 0x158 to 0x164 (Reserved)
    volatile unsigned int PCC_FlexIO;  // Offset 0x168
    unsigned int RESERVED_10[6];       // Offset 0x16C to 0x180 (Reserved)
    volatile unsigned int PCC_EWM;     // Offset 0x184
    unsigned int RESERVED_11[4];       // Offset 0x188 to 0x194 (Reserved)
    volatile unsigned int PCC_LPI2C0;  // Offset 0x198
    volatile unsigned int PCC_LPI2C1;  // Offset 0x19C
    unsigned int RESERVED_12[2];       // Offset 0x1A0 to 0x1A4 (Reserved)
    volatile unsigned int PCC_LPUART0; // Offset 0x1A8
    volatile unsigned int PCC_LPUART1; // Offset 0x1AC
    volatile unsigned int PCC_LPUART2; // Offset 0x1B0
    unsigned int RESERVED_13[1];       // Offset 0x1B4 (Reserved)
    volatile unsigned int PCC_FTM4;    // Offset 0x1B8
    volatile unsigned int PCC_FTM5;    // Offset 0x1BC
    volatile unsigned int PCC_FTM6;    // Offset 0x1C0
    volatile unsigned int PCC_FTM7;    // Offset 0x1C4
    unsigned int RESERVED_14[1];       // Offset 0x1C8 (Reserved)
    volatile unsigned int PCC_CMP0;    // Offset 0x1CC
    unsigned int RESERVED_15[2];       // Offset 0x1D0 to 0x1D4 (Reserved)
    volatile unsigned int PCC_QSPI;    // Offset 0x1D8
    unsigned int RESERVED_16[1];       // Offset 0x1DC (Reserved)
    volatile unsigned int PCC_ENET;    // Offset 0x1E4
} PCC_Type;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/** Peripheral PCC base address */
#define PCC_BASE_ADDR (0x40065000u)
/** Peripheral PCC base pointer */
#define PCC ((PCC_Type *)PCC_BASE_ADDR)

#endif