/**
*   @file    Gpio.h
*   @brief   Declaration of function prototypes and special parameters.
*   @details This file contains the definitions for structures, the Gpio_Type,
*          		the base address of GPIO, and measures to prevent multiple declarations.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef GPIO_H
#define GPIO_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "gpio_registers.h"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*!
 * @brief Initializes the pins with the given configuration structure
 *
 * This function configures the pins with the options provided in the
 * provided structure.
 *
 * @param[in] ConfigPtr:  The configuration structure
 * @return 
 */
typedef struct
{
    GPIO_Type         *      base;                  /*!< Gpio base pointer.  */
    unsigned char            GPIO_PinNumber;        /*!< Pin number.*/
    unsigned char            GPIO_PinMode;          /*!< Pin_mode */
} Gpio_ConfigType;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void Gpio_Init(const Gpio_ConfigType* ConfigPtr);
void GPIO_WriteToOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber, unsigned char value);
void GPIO_SetOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber);
void GPIO_ResetOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber);
void GPIO_ToggleOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber);
unsigned char GPIO_ReadFromInputPin(GPIO_Type *pGPIOx, unsigned char PinNumber);

#endif
