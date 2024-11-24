/**
*   @file    port.h
*   @brief   Header file for the PORT module.
*   @details This file provides the function prototypes, structure definitions, and macros required 
*            for configuring and utilizing the PORT peripheral. It defines the necessary data types 
*            and the initialization function for configuring the pins of the microcontroller.
*/

#ifndef PORT_H_
#define PORT_H_

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
/*!
 * @file     port.h
 * @brief    Header file for the PORT module interface.
 * @details  Includes definitions for port configurations and initialization functions.
 */
#include "port_registers.h"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/*!
 * @brief     Structure defining the configuration options for a PORT pin.
 * @details   This structure contains the settings for configuring a specific pin's
 *            mode, pull-up/down resistors, and interrupt options.
 */
typedef struct {
    unsigned int MUX : 3;           /*!< MUX control: Selects the function of the pin (GPIO, alternative functions, etc.). */
    unsigned int PullEnable: 1;     /*!< Pull Enable: Enables or disables the pull-up/pull-down resistors. */
    unsigned int PullUpDown: 1;     /*!< Pull Select: Configures the pin for pull-up (1) or pull-down (0). */
    unsigned int IQRC: 4;           /*!< Interrupt Configuration: Sets the interrupt trigger conditions. */
} Port_Mode_t;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void Port_Init(Port_Type *pPORTx, unsigned char PinNumber, Port_Mode_t Mode);

#endif /* PORT_H_ */
