/**
*   @file    Nvic_Registers.h
*   @brief   Definition of Register for NVIC module
*   @details Structure, NVIC_Type_t, NVIC_Base address, security to avoid multiple decleared
*/

/*==================================================================================================
==================================================================================================*/

#ifndef NVIC_REGISTER_H
#define NVIC_REGISTER_H

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          struct name NVIC_Type_t
* @details        This structure is for Nested Vectored Interrupt Controller Register and its offset
*
*/
/** NVIC - Register Layout Typedef */
typedef struct {
  volatile unsigned int 	NVIC_ISER[8];		/*OFFSET: 0x100, 0x104, 0x108, 0x10C, 0x110, 0x114, 0x118, 0x11C*/
	volatile unsigned int 	RESERVED1[24];
	volatile unsigned int 	NVIC_ICER[8];		/*OFFSET: 0x180, 0x184, 0x188, 0x18C, 0x190, 0x194, 0x198, 0x19C*/
	volatile unsigned int 	RESERVED2[24];
	volatile unsigned int 	NVIC_ISPR[8];		/*OFFSET: 0x200, 0x204, 0x208, 0x20C, 0x210, 0x214, 0x218, 0x21C*/
	volatile unsigned int 	RESERVED3[24];
	volatile unsigned int 	NVIC_ICPR[8];		/*OFFSET: 0x280, 0x284, 0x288, 0x28C, 0x290, 0x294, 0x298, 0x29C*/
	volatile unsigned int 	RESERVED4[24];
	volatile unsigned int 	NVIC_IABR[8];		/*OFFSET: 0x300, 0x304, 0x308, 0x30C, 0x310, 0x314, 0x318, 0x31C*/
	volatile unsigned int 	RESERVED5[56u];
	volatile unsigned char 	NVIC_IPR[240];	/*OFFSET: 0x400 -> 0x4EF*/
	volatile unsigned int 	RESERVED6[644];
	volatile unsigned int 	STIR;						/*OFFSET: 0xF00*/
} NVIC_Type_t;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/** NVIC base address */
#define NVIC_BASE                               (0xE000E100u)


/** NVIC base pointer */
#define NVIC    ((NVIC_Type_t *)NVIC_BASE)

#endif