/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "lpuart.h"
#include "pcc.h"
#include "systick.h"

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*!
 * @brief  Fingerprint module status codes.
 *
 * @detail Defines constants used to indicate the status of fingerprint operations.
 *
 */
#define FP_OK 0x00
#define FP_ERROR 0xFE
#define FP_NOFINGER 0x02
#define FP_FINGER_NOTMATCH 0x0A
#define FP_FINGER_NOTFOUND 0x09

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/

/*!
 * @brief  Predefined commands for the fingerprint module.
 *
 * @detail Arrays of commands to interact with the fingerprint sensor.
 *
 */

unsigned char FPHeader[6]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF};
unsigned char FPGetImage[6]={0x01,0x00,0x03,0x01,0x00,0x05};
unsigned char FPCreateCharFile1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08};
unsigned char FPCreateCharFile2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09};
unsigned char FPCreateCharFile3[7]={0x01,0x00,0x04,0x02,0x03,0x00,0x0A};
unsigned char FPCreateTemplate[6]={0x01,0x00,0x03,0x05,0x00,0x09};
unsigned char FPDeleteAllFinger[6]={0x01,0x00,0x03,0x0D,0x00,0x11};
unsigned char FPSearchFinger[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x40,0x00,0x4E};
unsigned char FPGetNumberOfFinger[6]={0x01,0x00,0x03,0x1D,0x00,0x21};

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Initializes the clock for the specified LPUART module.
 *
 * @detail    This function configures and enables the clock for the LPUART module
 *            by selecting the appropriate clock source.
 *
 * @param[in] PCC_LPUARTx Pointer to the PCC register for the specific LPUART module.
 * @return    void
 */
void LPUART_init_clock(volatile unsigned int* PCC_LPUARTx){
	PCC_SetClockSource(PCC_LPUARTx, 6); /*SPLL = 160MHz, Fclk = SPLL_DIV2 = 40MHz*/
	PCC_EnableClock(PCC_LPUARTx);
}

/*!
 * @brief     Configures the LPUART module for a baud rate of 9600.
 *
 * @detail    This function sets up the baud rate and other configurations such as
 *            data bits and stop bits for the LPUART module.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @return    void
 */
void LPUART_config_baud9600(LPUART_t* LPUARTx){
	/* BAUD rate = Fclk /(OSR *(SBR + 1)*/
	/* Fclk = 40000000, Baud rate = 9600, OSR = 16 -->  SBR = 260*/
	LPUARTx->BAUD.SBR = 260;
	LPUARTx->BAUD.OSR = 15;
	LPUARTx->BAUD.SBNS = 0; /*1 stop bit*/
	LPUARTx->CTRL.M = 0;		/*8 bit data*/
	LPUARTx->CTRL.PE = 0; 	/*No parity enable*/
}

/*!
 * @brief     Configures the LPUART module for a baud rate of 57600.
 *
 * @detail    Similar to LPUART_config_baud9600, but sets up the baud rate for 57600.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @return    void
 */
void LPUART_config_baud57600(LPUART_t* LPUARTx){
	/* BAUD rate = Fclk /(OSR *(SBR + 1)*/
	/* Fclk = 40000000, Baud rate = 57600, OSR = 16 -->  SBR = 42*/
	LPUARTx->BAUD.SBR = 42;
	LPUARTx->BAUD.OSR = 15;
	LPUARTx->BAUD.SBNS = 0; /*1 stop bit*/
	LPUARTx->CTRL.M = 0;		/*8 bit data*/
	LPUARTx->CTRL.PE = 0; 	/*No parity enable*/
}

/*!
 * @brief     Enables the LPUART transmitter.
 *
 * @detail    This function enables the transmitter on the LPUART module,
 *            allowing it to send data.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @return    void
 */
void LPUART_enable_transmitter(LPUART_t* LPUARTx){
	LPUARTx->CTRL.TE = 1;		/*Enable transmitter*/
}

/*!
 * @brief     Enables the LPUART receiver.
 *
 * @detail    This function enables the receiver on the LPUART module,
 *            allowing it to receive data.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @return    void
 */
void LPUART_enable_receiver(LPUART_t* LPUARTx){
	LPUARTx->CTRL.RE = 1;		/*Enable receiver*/
}

/*!
 * @brief     Sends a single byte via the LPUART module.
 *
 * @detail    This function sends a single byte of data through the LPUART module.
 *            It waits until the transmit buffer is empty before sending.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @param[in] send The byte to be sent.
 * @return    void
 */
void LPUART_send_byte(LPUART_t* LPUARTx, unsigned char send){
	while (!(LPUARTx->STAT.TDRE));
	LPUARTx->DATA_REGISTER |= send;
}

/*!
 * @brief     Sends a string of data via the LPUART module.
 *
 * @detail    This function sends a null-terminated string via the LPUART module.
 *            It sends one character at a time until it reaches the null terminator.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @param[in] data_string The string to be sent.
 * @return    void
 */
void LPUART_send_string(LPUART_t* LPUARTx, unsigned char data_string[])  {  /* Function to Transmit whole string */
	unsigned int i=0;
	while(data_string[i] != '\0')  {           /* Send chars one at a time */
		LPUART_send_byte(LPUARTx, data_string[i]);
		i++;
	}
}

/*!
 * @brief     Sends the fingerprint header command.
 *
 * @detail    This function sends the predefined fingerprint header command to the
 *            fingerprint module.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @return    void
 */
void sendFPHeader(LPUART_t* LPUARTx){
	unsigned char i;
	for(i = 0; i < 6; i++){
		LPUART_send_byte(LPUARTx, FPHeader[i]);
	}
}

/*!
 * @brief     Sends the 'Get Image' command to the fingerprint module.
 *
 * @detail    This function sends the 'Get Image' command to the fingerprint module
 *            and waits for an acknowledgment.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @param[in] ack Acknowledgment array from the fingerprint module.
 * @param[in] current_core_clock The current core clock frequency.
 * @return    The status of the fingerprint operation.
 */
unsigned char sendFPGetImage(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock){
	sendFPHeader(LPUARTx);
	unsigned char i;
	for(i = 0; i < 6; i++){
		LPUART_send_byte(LPUARTx, FPGetImage[i]);
	}
	delay(2000, current_core_clock);
	if(ack[1] == 0x07)
    return ack[4];
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief     Sends the 'Create Character File 1' command to the fingerprint module.
 *
 * @detail    This function sends the 'Create Character File 1' command to the fingerprint
 *            module and waits for an acknowledgment.
 *
 * @param[in] LPUARTx Pointer to the LPUART module.
 * @param[in] ack Acknowledgment array from the fingerprint module.
 * @param[in] current_core_clock The current core clock frequency.
 * @return    The status of the fingerprint operation.
 */
unsigned char sendFPCreateCharFile1(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock){
	sendFPHeader(LPUARTx);
	unsigned char i;
	for(i = 0; i < 7; i++){
		LPUART_send_byte(LPUARTx, FPCreateCharFile1[i]);
	}
	delay(2000, current_core_clock);
	if(ack[1] == 0x07)
    return ack[4];
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief Sends the "Create Character File 2" command to the fingerprint sensor.
 * @detail This function sends the "Create Character File 2" command and waits for an acknowledgment.
 * @param[in] LPUARTx Pointer to the LPUART instance.
 * @param[out] ack Array to store the acknowledgment response.
 * @param[in] current_core_clock The current core clock for delay calculation.
 * @return The result of the operation (e.g., FP_OK, FP_ERROR).
 */
unsigned char sendFPCreateCharFile2(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock){
	sendFPHeader(LPUARTx);
	unsigned char i;
	for(i = 0; i < 7; i++){
		LPUART_send_byte(LPUARTx, FPCreateCharFile2[i]);
	}
	delay(2000, current_core_clock);
	if(ack[1] == 0x07)
    return ack[4];
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief Sends the "Create Template" command to the fingerprint sensor.
 *
 * @detail This function sends the "Create Template" command, which combines two character files 
 *         into one template. The function waits for an acknowledgment after sending the command.
 *
 * @param[in] LPUARTx Pointer to the LPUART instance.
 * @param[out] ack Array to store the acknowledgment response.
 * @param[in] current_core_clock The current core clock for delay calculation.
 * @return The result of the operation (e.g., FP_OK, FP_ERROR).
 */
unsigned char sendFPCreateTemplate(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock){
	sendFPHeader(LPUARTx);
	unsigned char i;
	for(i = 0; i < 6; i++){
		LPUART_send_byte(LPUARTx, FPCreateTemplate[i]);
	}
	delay(2000, current_core_clock);
	if(ack[1] == 0x07)
    return ack[4];
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief Sends the "Delete All Fingerprints" command to the fingerprint sensor.
 *
 * @detail This function sends the command to delete all fingerprints stored in the fingerprint 
 *         sensor's memory. The function waits for an acknowledgment after sending the command.
 *
 * @param[in] LPUARTx Pointer to the LPUART instance.
 * @param[out] ack Array to store the acknowledgment response.
 * @param[in] current_core_clock The current core clock for delay calculation.
 * @return The result of the operation (e.g., FP_OK, FP_ERROR).
 */
unsigned char sendFPDeleteAllFinger(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock){
	sendFPHeader(LPUARTx);
	unsigned char i;
	for(i = 0; i < 6; i++){
		LPUART_send_byte(LPUARTx, FPDeleteAllFinger[i]);
	}
	delay(2000, current_core_clock);
	if(ack[1] == 0x07)
    return ack[4];
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief Sends the "Search Finger" command to the fingerprint sensor.
 *
 * @detail This function sends the "Search Finger" command to identify if the fingerprint matches 
 *         a stored template in the fingerprint sensor. If a match is found, the ID is sent via 
 *         LPUART. The function waits for an acknowledgment after sending the command.
 *
 * @param[in] LPUARTx Pointer to the LPUART instance.
 * @param[out] ack Array to store the acknowledgment response.
 * @param[in] current_core_clock The current core clock for delay calculation.
 * @return The result of the operation (e.g., FP_OK, FP_ERROR).
 */
unsigned char sendFPSearchFinger(LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock){
	sendFPHeader(LPUARTx);
	unsigned char i;
	for(i = 0; i < 11; i++){
		LPUART_send_byte(LPUARTx, FPSearchFinger[i]);
	}
	delay(5000, current_core_clock);
	if(ack[1] == 0x07){
		if(ack[3] == 0x07){
			LPUART_send_byte(LPUART1, ack[5] + 0x30);
			LPUART_send_byte(LPUART1, 0x0A);
			LPUART_send_byte(LPUART1, ack[6] + 0x30);
			LPUART_send_byte(LPUART1, 0x0A);
    return ack[4];
		}
		else return FINGERPRINT_UNDEFINED_ERROR;
	}
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief Sends the "Store Fingerprint" command to the fingerprint sensor.
 *
 * @detail This function sends the command to store the fingerprint template at a specified 
 *         ID in the fingerprint sensor's memory. The function waits for an acknowledgment 
 *         after sending the command.
 *
 * @param[in] IDStore The ID at which to store the fingerprint template.
 * @param[in] LPUARTx Pointer to the LPUART instance.
 * @param[out] ack Array to store the acknowledgment response.
 * @param[in] current_core_clock The current core clock for delay calculation.
 * @return The result of the operation (e.g., FP_OK, FP_ERROR).
 */
unsigned char SendStoreFinger(unsigned char IDStore, LPUART_t* LPUARTx, unsigned char ack[], unsigned int current_core_clock)
{
	sendFPHeader(LPUARTx);
	unsigned char Sum= 0x01 + 0x00 + 0x06 + 0x06 + 0x01 +0x00 + IDStore;;
	LPUART_send_byte(LPUARTx, 0x01);
	LPUART_send_byte(LPUARTx, 0x00);
	LPUART_send_byte(LPUARTx, 0x06);
	LPUART_send_byte(LPUARTx, 0x06);
	LPUART_send_byte(LPUARTx, 0x01);
	LPUART_send_byte(LPUARTx, 0x00);
	LPUART_send_byte(LPUARTx, IDStore);
	LPUART_send_byte(LPUARTx, 0x00);
	LPUART_send_byte(LPUARTx, Sum);
	delay(2000, current_core_clock);
	if(ack[1] == 0x07)
    return ack[4];
  else return FINGERPRINT_UNDEFINED_ERROR;
}

/*!
 * @brief Checks the fingerprint sensor's response and prints corresponding messages via LPUART.
 *
 * @detail This function interprets the response code from the fingerprint sensor and prints 
 *         an appropriate message over LPUART1. It handles various error and success cases.
 *
 * @param[in] response The response code from the fingerprint sensor.
 * @return void
 */
void check_response_fingerprint(unsigned char response) {
    switch (response) {
        case FINGERPRINT_OK:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_OK");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_RECEIVE_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_RECEIVE_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_NO_FINGER:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_NO_FINGER");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_IMAGE_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_IMAGE_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_IMAGE_TOO_LIGHT:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_IMAGE_TOO_LIGHT");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_IMAGE_TOO_BLURRY:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_IMAGE_TOO_BLURRY");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_IMAGE_AMORPHOUS:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_IMAGE_AMORPHOUS");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_IMAGE_TOO_SMALL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_IMAGE_TOO_SMALL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_UNMATCHED:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_UNMATCHED");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_NO_SEARCH:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_NO_SEARCH");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_MERGE_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_MERGE_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_ADDRESS_SN_OUT_OF_RANGE:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_ADDRESS_SN_OUT_OF_RANGE");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_TEMPLATE_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_TEMPLATE_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_UPLOAD_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_UPLOAD_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_CONTINUE_PACKET_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_CONTINUE_PACKET_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_IMAGE_UPLOAD_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_IMAGE_UPLOAD_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_DELETE_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_DELETE_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_DB_CLEAR_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_DB_CLEAR_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_LOW_POWER_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_LOW_POWER_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_PASSWORD_INCORRECT:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_PASSWORD_INCORRECT");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_RESET_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_RESET_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_NO_VALID_IMAGE:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_NO_VALID_IMAGE");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_UPGRADE_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_UPGRADE_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_INCOMPLETE:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_INCOMPLETE");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_FLASH_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_FLASH_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_CONTINUE_ACK_0XF0:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_CONTINUE_ACK_0XF0");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_CONTINUE_ACK_0XF1:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_CONTINUE_ACK_0XF1");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_SUM_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_SUM_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_PACKET_FLAG_ERROR:
           LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_PACKET_FLAG_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_PACKET_LENGTH_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_PACKET_LENGTH_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_CODE_LENGTH_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_CODE_LENGTH_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_FLASH_BURN_FAIL:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_FLASH_BURN_FAIL");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        case FINGERPRINT_UNDEFINED_ERROR:
            LPUART_send_string(LPUART1, (unsigned char*)"FINGERPRINT_UNDEFINED_ERROR");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
        default:
            LPUART_send_string(LPUART1, (unsigned char*)"UNKNOWN_RESPONSE_CODE");
						LPUART_send_byte(LPUART1, 0x0A);
            break;
    }
}
