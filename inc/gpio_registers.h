/**
*   @file    Gpio_Registers.h
*   @brief   Definition of Register for GPIO module
*   @details Structure, Gpio_Type, Gpio_Base address, security to avoid multiple decleared
*/

/*==================================================================================================
==================================================================================================*/

#ifndef GPIO_REGISTER_H
#define GPIO_REGISTER_H

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          struct name GPIO_Type
* @details        This structure is for GPIO Register and its offset
*
*/
/** GPIO - Register Layout Typedef */
typedef struct {
  volatile unsigned int PDOR;
	volatile unsigned int PSOR;
	volatile unsigned int PCOR;
	volatile unsigned int PTOR;
	volatile const unsigned int PDIR;
	volatile unsigned int PDDR;
	volatile unsigned int PIDR;
} GPIO_Type;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/** Peripheral GPIOA base address */
#define GPIOA_BASE                               (0x400FF000u)
#define GPIOB_BASE                               (0x400FF040u)
#define GPIOC_BASE                               (0x400FF080u)
#define GPIOD_BASE                               (0x400FF0C0u)
#define GPIOE_BASE                               (0x400FF100u)


/** Peripheral GPIOA base pointer */
#define GPIOA    ((GPIO_Type *)GPIOA_BASE)
#define GPIOB    ((GPIO_Type *)GPIOB_BASE)
#define GPIOC    ((GPIO_Type *)GPIOC_BASE)
#define GPIOD    ((GPIO_Type *)GPIOD_BASE)
#define GPIOE    ((GPIO_Type *)GPIOE_BASE)

#endif