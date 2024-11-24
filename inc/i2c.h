/**
*   @file    I2C.h
*   @brief   Declaration of function prototypes and configuration parameters for I2C module.
*   @details This file provides the function declarations for initializing and controlling the I2C 
*            module, as well as other utility functions related to I2C communication. It also includes
*            necessary measures to prevent multiple declarations.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef I2C_H_
#define I2C_H_

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "i2c_registers.h"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*!
 * @brief Initializes the LPI2C0 module with the specified configuration
 *
 * This function sets up the LPI2C0 module using the default or specified
 * configurations provided in the structure.
 *
 * @param[in] ConfigPtr: The configuration structure (if applicable)
 * @return None
 */

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

void init_LPI2C0(void);
unsigned char bus_busy(void);
void generate_start_ACK(unsigned char address);
void transmit_data(unsigned char data);
unsigned char generate_stop(void);
unsigned char LPI2C0_write(unsigned char s_w_address, unsigned char s_reg_address, unsigned char byte);
unsigned char LPI2C0_read(unsigned char s_w_address, unsigned char s_reg_address);
void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_clear(void);
void lcd_clear_rtc(void);
void lcd_init(void);
void lcd_send_string(char *str);
void lcd_put_cur(unsigned char row, unsigned char col);

#endif /* I2C_H_ */
