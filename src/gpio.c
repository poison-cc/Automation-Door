/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "gpio.h"
 
 /*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*!
 * @brief  Pin mode constants.
 *
 * @detail Defines constants for pin modes to avoid using hardcoded values.
 *
 */
#define PIN_MODE_INPUT  0
#define PIN_MODE_OUTPUT 1


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Initializes the pins with the given configuration structure
 *
 * @detail    This function configures the pins with the options provided in the
 *            provided structure.
 *
 * @param[in] pinCount The number of configured pins in structure
 * @param[in] config The configuration structure
 * @return    void
 */
void Gpio_Init(const Gpio_ConfigType* ConfigPtr)
{
    /* Check parameter */
	if(ConfigPtr->GPIO_PinMode == PIN_MODE_OUTPUT)
    ConfigPtr->base->PDDR |= (1U << ConfigPtr->GPIO_PinNumber);
	else if(ConfigPtr->GPIO_PinMode == PIN_MODE_INPUT){
		ConfigPtr->base->PDDR &= ~(1U << ConfigPtr->GPIO_PinNumber);
	}else{
		/*Error mode*/
		return;	/*do nothing*/
	}
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Writes a value to a specified pin.
 *
 * @detail    This function writes the given value to the specified pin.
 *
 * @param[in] pGPIOx Pointer to the base address of the GPIO.
 * @param[in] PinNumber The position of the pin in its port, ranging from 0 to 31.
 * @param[in] value The value to be driven to the pin.
 * @return    void
 */
void GPIO_WriteToOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber, unsigned char value)
{
    /* Check parameter */
	if(value == 1)
    pGPIOx->PDOR |= (1U << PinNumber);
	else if(value == 0){
		pGPIOx->PDOR &= ~(1U << PinNumber);
	}else{
		/*Error mode -> do nothing*/
		return;
	}
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Sets a pin configured as output to HIGH.
 *
 * @detail    This function sets the specified pin, configured as output, to HIGH.
 *
 * @param[in] pGPIOx Pointer to the base address of the GPIO.
 * @param[in] PinNumber The position of the pin in its port, ranging from 0 to 31.
 * @return    void
 */
void GPIO_SetOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber)
{
    /* Check parameter */
	pGPIOx->PDOR |= (1U << PinNumber);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Sets a pin configured as output to LOW.
 *
 * @detail    This function sets the specified pin, configured as output, to LOW.
 *
 * @param[in] pGPIOx Pointer to the base address of the GPIO.
 * @param[in] PinNumber The position of the pin in its port, from 0 to 31.
 * @return    void
 */

void GPIO_ResetOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber)
{
    /* Check parameter */
		pGPIOx->PDOR &= ~(1U << PinNumber);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Inverts the value of a pin and drives that value to the output.
 *
 * @detail    This function inverts the current value of the specified pin and
 *            then drives that inverted value to the pin configured as output.
 *
 * @param[in] pGPIOx Pointer to the base address of the GPIO.
 * @param[in] PinNumber The position of the pin in its port, ranging from 0 to 31.
 * @return    void
 */

void GPIO_ToggleOutputPin(GPIO_Type *pGPIOx, unsigned char PinNumber)
{
    /* Check parameter */
    pGPIOx->PDOR ^= (1u << PinNumber);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Reads the value of a pin configured as input.
 *
 * @detail    This function reads and returns the current value of the specified pin,
 *            which is configured as input mode.
 *
 * @param[in] pGPIOx Pointer to the base address of the GPIO.
 * @param[in] PinNumber The position of the pin in its port, ranging from 0 to 31.
 * @return    unsigned char.
 */
unsigned char GPIO_ReadFromInputPin(GPIO_Type *pGPIOx, unsigned char PinNumber)
{
    /* Check parameter */
	return (pGPIOx->PDIR & (1U << PinNumber));
}
