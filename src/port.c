/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "port.h"

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*!
 * @brief  Initializes the related registers that are not contained in the PORT module.
 *
 * @detail These definitions must be added to configure the clock source for PORT
 *         and set the mode for the PORT module.
 */

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Initializes the pins with the provided configuration structure.
 *
 * @detail    This function configures the pins with the options specified in the
 *            given configuration structure.
 *
 * @param[in] pPORTx Pointer to the base address of the PORT peripheral
 * @param[in] PinNumber The pin number to be configured
 * @param[in] Mode The configuration structure containing pin settings
 * @return    void
 */
void Port_Init(Port_Type *pPORTx, unsigned char PinNumber, Port_Mode_t Mode) {
    pPORTx->PCR[PinNumber].PORT_PCR_Fields.PE = Mode.PullEnable;
    pPORTx->PCR[PinNumber].PORT_PCR_Fields.PS = Mode.PullUpDown;
    pPORTx->PCR[PinNumber].PORT_PCR_Fields.MUX = Mode.MUX;
    pPORTx->PCR[PinNumber].PORT_PCR_Fields.IQRC = Mode.IQRC;
}

