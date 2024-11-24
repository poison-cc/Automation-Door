/**
*   @file    pcc.c
*   @brief   Implementation of functions for the PCC module.
*   @details This file contains the implementations of functions to control the clock
*            settings for various peripherals via the Peripheral Clock Control (PCC) module.
*/

/*==================================================================================================
*                                       INCLUDE FILES
==================================================================================================*/
#include "pcc.h"

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*!
 * @brief     Enables the clock for the specified peripheral by setting the CGC bit.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @return    void
 */
void PCC_EnableClock(volatile unsigned int *pccRegister) {
    *pccRegister |= (1U << 30); // Set the CGC bit to enable the clock
}

/*!
 * @brief     Disables the clock for the specified peripheral by clearing the CGC bit.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @return    void
 */
void PCC_DisableClock(volatile unsigned int *pccRegister) {
    *pccRegister &= ~(1U << 30); // Clear the CGC bit to disable the clock
}

/*!
 * @brief     Sets the clock source for the specified peripheral.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @param[in] clockSource The clock source to be set (3-bit value).
 * @return    void
 */
void PCC_SetClockSource(volatile unsigned int *pccRegister, unsigned int clockSource) {
    *pccRegister = (*pccRegister & ~(0x7U << 24)) | (clockSource << 24);
}

/*!
 * @brief     Sets the clock fraction for the specified peripheral.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @param[in] clockFraction The clock fraction to be set (1-bit value).
 * @return    void
 */
void PCC_SetClockFraction(volatile unsigned int *pccRegister, unsigned int clockFraction) {
    *pccRegister = (*pccRegister & ~(0x1U << 3)) | (clockFraction << 3);
}

/*!
 * @brief     Selects the clock divider for the specified peripheral.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @param[in] clockDivider The clock divider to be set (2-bit value).
 * @return    void
 */
void PCC_SelectClockDivider(volatile unsigned int *pccRegister, unsigned int clockDivider) {
    *pccRegister = (*pccRegister & ~(0x3U)) | (clockDivider);
}

/*!
 * @brief     Gets the current clock status for the specified peripheral.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @return    The status of the CGC (Clock Gate Control) bit (1 if enabled, 0 if disabled).
 */
unsigned int PCC_GetClockStatus(volatile unsigned int *pccRegister) {
    return (*pccRegister & (1U << 30)) >> 30; // Extract the CGC bit
}

/*!
 * @brief     Checks if the specified peripheral is present by examining the peripheral present bit.
 * @param[in] pccRegister Pointer to the PCC register for the peripheral.
 * @return    1 if the peripheral is present, 0 otherwise.
 */
unsigned int PCC_IsPeripheralPresent(volatile unsigned int *pccRegister) {
    return (*pccRegister & (1U << 31)) >> 31;
}
