/**
*   @file    LPUART.h
*   @brief   Declaration of LPUART functions and parameters
*   @details This file contains the declarations for LPUART (Low Power UART) functions and configuration types.
*            It includes definitions for initializing the LPUART clock, configuring baud rates, enabling
*            transmitters and receivers, sending and receiving bytes and strings, and handling fingerprint
*            sensor communication. It also defines an enumeration for various fingerprint sensor response codes.
*            Measures are included to prevent multiple declarations using include guards.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef LPUART_H
#define LPUART_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "lpuart_registers.h"

/*==================================================================================================
*                                    ENUMERATIONS
==================================================================================================*/

/*!
 * @brief Fingerprint sensor response codes
 *
 * This enumeration defines various response codes returned by the fingerprint sensor.
 */
typedef enum {
    FINGERPRINT_OK = 0x00,                    /*!< Indicates instruction implementing end or OK */
    FINGERPRINT_RECEIVE_ERROR = 0x01,         /*!< Indicates data packet receiving error */
    FINGERPRINT_NO_FINGER = 0x02,             /*!< Indicates no finger on the sensor */
    FINGERPRINT_IMAGE_FAIL = 0x03,            /*!< Indicates getting fingerprint image failed */
    FINGERPRINT_IMAGE_TOO_LIGHT = 0x04,       /*!< Indicates the fingerprint image is too dry or too light to generate feature */
    FINGERPRINT_IMAGE_TOO_BLURRY = 0x05,      /*!< Indicates the fingerprint image is too humid or too blurry to generate feature */
    FINGERPRINT_IMAGE_AMORPHOUS = 0x06,       /*!< Indicates the fingerprint image is too amorphous to generate feature */
    FINGERPRINT_IMAGE_TOO_SMALL = 0x07,       /*!< Indicates the fingerprint image is in order, but with too little minutiae or too small area to generate feature */
    FINGERPRINT_UNMATCHED = 0x08,             /*!< Indicates the fingerprint unmatched */
    FINGERPRINT_NO_SEARCH = 0x09,             /*!< Indicates no fingerprint searched */
    FINGERPRINT_MERGE_FAIL = 0x0A,            /*!< Indicates the feature merging failed */
    FINGERPRINT_ADDRESS_SN_OUT_OF_RANGE = 0x0B, /*!< Indicates the address SN exceeding the range of fingerprint database when accessing it */
    FINGERPRINT_TEMPLATE_ERROR = 0x0C,        /*!< Indicates template reading error or invalid from the fingerprint database */
    FINGERPRINT_UPLOAD_FAIL = 0x0D,           /*!< Indicates feature uploading failed */
    FINGERPRINT_CONTINUE_PACKET_FAIL = 0x0E,  /*!< Indicates the module cannot receive continue data packet */
    FINGERPRINT_IMAGE_UPLOAD_FAIL = 0x0F,     /*!< Indicates image uploading failed */
    FINGERPRINT_DELETE_FAIL = 0x10,           /*!< Indicates module deleting failed */
    FINGERPRINT_DB_CLEAR_FAIL = 0x11,         /*!< Indicates the fingerprint database clearing failed */
    FINGERPRINT_LOW_POWER_FAIL = 0x12,        /*!< Indicates cannot be in low power consumption */
    FINGERPRINT_PASSWORD_INCORRECT = 0x13,    /*!< Indicates the password incorrect */
    FINGERPRINT_RESET_FAIL = 0x14,            /*!< Indicates the system reset failed */
    FINGERPRINT_NO_VALID_IMAGE = 0x15,        /*!< Indicates there is no valid original image in buffer to generate image */
    FINGERPRINT_UPGRADE_FAIL = 0x16,          /*!< Indicates on-line upgrading failed */
    FINGERPRINT_INCOMPLETE = 0x17,            /*!< Indicates there are incomplete fingerprint or finger stay still between twice image capturing */
    FINGERPRINT_FLASH_ERROR = 0x18,           /*!< Indicates read-write FLASH error */
    FINGERPRINT_CONTINUE_ACK_0XF0 = 0xF0,     /*!< Existing instruction of continue data packet, ACK with 0xf0 after receiving correctly */
    FINGERPRINT_CONTINUE_ACK_0XF1 = 0xF1,     /*!< Existing instruction of continue data packet, the command packet ACK with 0xf1 */
    FINGERPRINT_SUM_ERROR = 0xF2,             /*!< Indicates Sum error when burning internal FLASH */
    FINGERPRINT_PACKET_FLAG_ERROR = 0xF3,     /*!< Indicates packet flag error when burning internal FLASH */
    FINGERPRINT_PACKET_LENGTH_ERROR = 0xF4,   /*!< Indicates packet length error when burning internal FLASH */
    FINGERPRINT_CODE_LENGTH_ERROR = 0xF5,     /*!< Indicates the code length too long when burning internal FLASH */
    FINGERPRINT_FLASH_BURN_FAIL = 0xF6,       /*!< Indicates burning FLASH failed when burning internal FLASH */
    FINGERPRINT_UNDEFINED_ERROR = 0x19        /*!< Non-defined error */
} fingerprint_response_t;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void LPUART_init_clock(volatile unsigned int* PCC_LPUARTx);
void LPUART_config_baud9600(LPUART_t* LPUARTx);
void LPUART_config_baud57600(LPUART_t* LPUARTx);
void LPUART_enable_transmitter(LPUART_t* LPUARTx);
void LPUART_enable_receiver(LPUART_t* LPUARTx);
void LPUART_send_byte(LPUART_t* LPUARTx, unsigned char send);
void LPUART_send_string(LPUART_t* LPUARTx, unsigned char data_string[]);
char LPUART_receive_char(LPUART_t* LPUARTx);
void lpuart_receive_string(LPUART_t* LPUARTx, unsigned char* buffer, unsigned int* buffer_index, unsigned int buffer_size);
void sendFPHeader(LPUART_t* LPUARTx);
unsigned char sendFPGetImage(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
unsigned char sendFPCreateCharFile1(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
unsigned char sendFPCreateCharFile2(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
unsigned char sendFPCreateTemplate(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
unsigned char sendFPDeleteAllFinger(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
unsigned char sendFPSearchFinger(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
unsigned char SendStoreFinger(unsigned char IDStore, LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock);
void check_response_fingerprint(unsigned char response) ;

#endif