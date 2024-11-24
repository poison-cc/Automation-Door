/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "nvic.h"

/*==================================================================================================
*                                      ``LOCAL MACRO
==================================================================================================*/
#define NVIC_GROUP_SIZE 5    /* Shift 5 bits to receive the number of register */
#define NVIC_BIT_MASK 0x1F   /* Mask to define the exact bit field of the register */

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Enable interrupt
 *
 * @detail    Enable Nested Vectored Interrupt Controller by IRQ number, from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    void
 */
void NVIC_EnableIRQ(unsigned int IRQn) {
  NVIC->NVIC_ISER[IRQn >> NVIC_GROUP_SIZE] |= (1 << (IRQn & NVIC_BIT_MASK));
}

/*!
 * @brief     Disable interrupt
 *
 * @detail    Disable Nested Vectored Interrupt Controller by IRQ number, from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    void
 */
void NVIC_DisableIRQ(unsigned int IRQn){
	NVIC->NVIC_ICER[IRQn >> NVIC_GROUP_SIZE] |= (1 << (IRQn & NVIC_BIT_MASK));
}

/*!
 * @brief     SetPending interrupt
 *
 * @detail    SetPending Nested Vectored Interrupt Controller by IRQ number, from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    void
 */
void NVIC_SetPendingIRQ(unsigned int IRQn){
	NVIC->NVIC_ISPR[IRQn >> NVIC_GROUP_SIZE] |= (1 << (IRQn & NVIC_BIT_MASK));
}

/*!
 * @brief     ClearPending interrupt
 *
 * @detail    ClearPending Nested Vectored Interrupt Controller by IRQ number, from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    void
 */
void NVIC_ClearPendingIRQ(unsigned int IRQn){
	NVIC->NVIC_ICPR[IRQn >> NVIC_GROUP_SIZE] |= (1 << (IRQn & NVIC_BIT_MASK));
}

/*!
 * @brief     Get state active or inactive of interrupt
 *
 * @detail    Get state Actived or Inactive of Nested Vectored Interrupt Controller by IRQ number, 
 * 						from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    unsigned int
 */
unsigned int NVIC_GetActive(unsigned int IRQn) {
    return (NVIC->NVIC_IABR[IRQn >> NVIC_GROUP_SIZE] & (1 << (IRQn & NVIC_BIT_MASK))) ? 1 : 0;
}

/*!
 * @brief     Set priorty of interrupt specificed.
 *
 * @detail    Set priorty of Nested Vectored Interrupt Controller by IRQ number, from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    void
 */
void NVIC_SetPriority(unsigned int IRQn, unsigned int priority) {
	if(priority < 16u){
		NVIC->NVIC_IPR[(unsigned int)IRQn] = (priority << 4);
	}else{
		/*Do nothing/Error notification*/
	}
}

/*!
 * @brief     Get priorty of interrupt specificed.
 *
 * @detail    Get priorty of Nested Vectored Interrupt Controller by IRQ number, from 0 to 112
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    unsigned int
 */
unsigned int NVIC_GetPriority(unsigned int IRQn) {
    return (NVIC->NVIC_IPR[IRQn >> 2] >> ((IRQn & 0x3) * 8)) & 0xFF;
}

/*!
 * @brief     Software trigger
 *
 * @detail    Trigger interrupt of IRQn to debugging
 *
 * @param[in] IRQn The Interrupt Request Numbers defined by enum IRQn_Type
 *
 * @return    void
 */
void NVIC_TriggerIRQ(unsigned int IRQn){
		NVIC->STIR = IRQn;
}

