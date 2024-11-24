/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "pcc.h"
#include "port.h"
#include "lpuart.h"
#include "gpio.h"
#include "clock.h"
#include "systick.h"
#include "nvic.h"
#include "i2c.h"
#include "RTC.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define CORE_CLOCK 80000000U
#define IMPORT_FINGERPRINT_MODE 0
#define SEARCH_FINGERPRINT_MODE 1
#define CREATE_NEW_USER_NAME_MODE 2
#define KEYTAP_NUMBER_MODE 0
#define KEYTAP_CHARACTER_MODE 1
#define MAX_NUM_USER 100
#define MAX_NAME_LENGTH 16

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void configC6();
void configC7();
void configA2();
void configA3();
void configD6();
void configD7();
void config_PTC8();
void config_PTC9();
void config_PTC10();
void config_PTC11();
void config_PTC1();
void config_PTC2();
void config_PTC16();
void config_PTC15();
void configD1();
void init_systick();
unsigned char check_but();
unsigned char value = 0;
unsigned char get_key();
void scan_row(unsigned char r);
unsigned char check_col();
void handle_keytap();
void configD15();
void init_clock();
void init_nvic();
void init_pcc();
void init_pinout();
void init_lpuart();
void init_flash();
void display_time();
void import_finger_print();
void search_finger_print();

/*==================================================================================================
*                                       STATIC VARIABLES
==================================================================================================*/
static unsigned char MODE = 0;
static unsigned char button_press_count = 0;
static unsigned char cursor_position = 0;
static char name_user[MAX_NUM_USER][MAX_NAME_LENGTH];
static unsigned char finger_mode = SEARCH_FINGERPRINT_MODE;
static unsigned char IDStore = 0;
static unsigned char data_ack[16] = {0};  // Buffer to store acknowledged data
static unsigned char data_ack_index = 0;  // Index to keep track of data_ack position
static unsigned int second = 0;
static unsigned int minute = 0;
static unsigned int hour = 0;
static unsigned int totalSeconds;
static unsigned int totalMinutes;
static unsigned int totalHours;
static unsigned char base_second = 0;
static unsigned char base_minute = 31;
static unsigned char base_hour = 15;

/*==================================================================================================
*                                       MAIN FUNCTION
==================================================================================================*/
int main(){
	init_clock();
	init_pcc();
	init_pinout();
	init_lpuart();
	init_systick();
	init_LPI2C0();
	RTC_init();
	init_nvic();	
	lcd_init();
	lcd_clear();
	lcd_put_cur(0, 0);
	init_flash();
	while(1){
		if(finger_mode == IMPORT_FINGERPRINT_MODE){
			import_finger_print();
		}
		else if(finger_mode == SEARCH_FINGERPRINT_MODE){
			search_finger_print();
		}
		else if (finger_mode == CREATE_NEW_USER_NAME_MODE){
			handle_keytap();
			delay(200, CORE_CLOCK);
		}
	}
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*!
 * @brief     Handles the LPUART2 receive and transmit interrupt.
 *
 * @detail    This interrupt service routine reads incoming data from the LPUART2,
 *            stores it in a buffer, and checks for a specific header sequence. If
 *            the header sequence is detected, it resets the data_ack buffer and index.
 *            Incoming data is also stored in the data_ack buffer if space allows.
 *            The buffer index wraps around when it exceeds the buffer size.
 *
 * @param[in]  None
 * @return     void
 */
void LPUART2_RxTx_IRQHandler(void) {
	static unsigned char buffer[20] = {0};  /* Buffer to store incoming data */
	static unsigned char buffer_index = 0;  /* Index to keep track of buffer position */
	static const unsigned char header[6] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};

	if (LPUART2->STAT.RDRF) {
		unsigned char incomingByte = LPUART2->DATA_REGISTER;
		buffer[buffer_index] = incomingByte;  /* Store incoming byte in buffer */
		/* Check if buffer has at least 6 bytes for comparison */
		if (buffer_index >= 5) {
				/* Check if the last six bytes match the header */
				if (memcmp(&buffer[buffer_index - 5], header, 6) == 0) {
						/* Reset data_ack and data_ack_index for new data */
						memset(data_ack, 0, sizeof(data_ack));
						data_ack_index = 0;
				}
		}
		/* Store incoming byte in data_ack if index is within limit */
		if (data_ack_index < sizeof(data_ack)) {
				data_ack[data_ack_index] = incomingByte;
				data_ack_index++;
		}
		buffer_index++;  /* Increment buffer index */
		/* Wrap around if buffer_index exceeds the size of the buffer */
		if (buffer_index >= sizeof(buffer)) {
				buffer_index = 0;
		}
	}
}

/*!
 * @brief     Handles the LPUART1 receive and transmit interrupt.
 *
 * @detail    This interrupt service routine reads the received byte from the LPUART1
 *            and updates the IDStore variable. Based on the value of the received byte,
 *            it sets the `finger_mode` to either `IMPORT_FINGERPRINT_MODE` or
 *            `SEARCH_FINGERPRINT_MODE`. If the received byte is less than 99, it sets
 *            the mode to import fingerprints; if greater than 99, it sets the mode to
 *            search for fingerprints.
 *
 * @param[in]  None
 * @return     void
 */
void LPUART1_RxTx_IRQHandler(void){
	if (LPUART1->STAT.RDRF) {
			unsigned char received = LPUART1->DATA_REGISTER;
			IDStore = received;
		if(received < 99){
			finger_mode = IMPORT_FINGERPRINT_MODE;
		}else if (received > 99){
			finger_mode = SEARCH_FINGERPRINT_MODE;
		}
	}
}

/*!
 * @brief     Handles the RTC seconds interrupt.
 *
 * @detail    This interrupt service routine calculates the total elapsed time in seconds
 *            since the base time, which is stored in `base_hour`, `base_minute`, and
 *            `base_second`. It updates the `second`, `minute`, and `hour` variables based
 *            on the total elapsed time. If the `hour` value equals 24, it resets the RTC
 *            timestamp register (TSR) to 0.
 *
 * @param[in]  None
 * @return     void
 */
void RTC_Seconds_IRQHandler()
{
	totalSeconds = base_hour * 3600 + base_minute * 60 + base_second + RTC->TSR;
	second = totalSeconds % 60;
	totalMinutes = totalSeconds / 60;
	minute = totalMinutes % 60;
	totalHours = totalMinutes / 60;
	hour = totalHours % 24;
	if (hour == 24)
	{
		RTC->TSR = 0;
	}
}

/*!
 * @brief     Displays the current time on the LCD.
 *
 * @detail    This function formats the current time into a string in the format "HH:MM:SS",
 *            where HH is hours, MM is minutes, and SS is seconds. It then clears the LCD display
 *            and prints the formatted time string at a specific position on the LCD.
 *
 * @param[in]  None
 * @return     void
 */
void display_time(){
	char time_str[9];
  snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", hour, minute, second);
  lcd_clear_rtc();
  lcd_put_cur(1, 8);
  lcd_send_string(time_str);
}

/*!
 * @brief     Initializes the system clock settings.
 *
 * @detail    This function configures the system clock by initializing the system oscillator
 *            to 8 MHz, setting up the SPLL (System Phase-Locked Loop) to 160 MHz using the
 *            8 MHz oscillator, and setting the clocks to 80 MHz for the system and core,
 *            40 MHz for the bus, and 20 MHz for the flash. It also sets the FIRCDIV (Fixed
 *            Internal Reference Clock Divider) to a division factor of 2.
 *
 * @param[in]  None
 * @return     void
 */
void init_clock(){
	SOSC_init_8MHz();
	SPLL_init_160MHz();
	NormalRUNmode_80MHz();
	SCG->FIRCDIV_bits.FIRCDIV_FIRCDIV2 = (1U);
}

/*!
 * @brief     Initializes the Nested Vectored Interrupt Controller (NVIC).
 *
 * @detail    This function enables interrupts for LPUART1, LPUART2, and RTC seconds.
 *            It sets up the NVIC to handle these interrupts, allowing the respective
 *            interrupt service routines to be triggered when the associated events occur.
 *
 * @param[in]  None
 * @return     void
 */
void init_nvic(){
	NVIC_EnableIRQ(IRQ_LPUART1_RXTX);
	NVIC_EnableIRQ(IRQ_LPUART2_RXTX);
	NVIC_EnableIRQ(IRQ_RTC_SECONDS);
}

/*!
 * @brief     Initializes the Peripheral Clock Controller (PCC).
 *
 * @detail    This function configures and enables the clocks for various peripherals.
 *            It enables the clocks for PORTC, PORTA, and PORTD, sets the clock source
 *            for LPUART1 and LPUART2 to SPLL at 80 MHz, and for LPI2C0 to SIRC divided by 2.
 *            It also enables the clock for the RTC peripheral.
 *
 * @param[in]  None
 * @return     void
 */
void init_pcc(){
	PCC_EnableClock(&PCC->PCC_PORTC);
	PCC_EnableClock(&PCC->PCC_PORTA);
	PCC_EnableClock(&PCC->PCC_PORTD);
	
	PCC_SetClockSource(&PCC->PCC_LPUART1, 6); /*SPLL 80MHz*/
	PCC_EnableClock(&PCC->PCC_LPUART1);
	PCC_SetClockSource(&PCC->PCC_LPUART2, 6); /*SPLL 80MHz*/
	PCC_EnableClock(&PCC->PCC_LPUART2);
	PCC_SetClockSource(&PCC->PCC_LPI2C0, 2); /*SIRC_DIV2*/
	PCC_EnableClock(&PCC->PCC_LPI2C0);
	
	PCC_EnableClock(&PCC->PCC_RTC);
}

/*!
 * @brief     Configures the pinout for various GPIO pins.
 *
 * @detail    This function initializes the pin configurations for multiple GPIO pins
 *            by calling specific configuration functions for each pin. The pin configuration
 *            functions configure pins C6, C7, A2, A3, D6, D7, D15, and several pins on PORTC 
 *            and PORTD, setting up their respective modes and functionalities.
 *
 * @param[in]  None
 * @return     void
 */
void init_pinout(){
	configC6();
	configC7();
	configA2();
	configA3();
	configD6();
	configD7();
	configD15();
	config_PTC8();
	config_PTC9();
	config_PTC10();
	config_PTC11();
	config_PTC1();
	config_PTC2();
	config_PTC16();
	config_PTC15();
	configD1();
}

/*!
 * @brief     Initializes the LPUART1 and LPUART2 modules.
 *
 * @detail    This function configures the baud rate for both LPUART1 and LPUART2 to 57,600
 *            baud using the `LPUART_config_baud57600` function. It enables the receiver
 *            interrupt for both LPUART modules and activates the transmitter and receiver
 *            for each LPUART. This setup allows for communication via LPUART1 and LPUART2.
 *
 * @param[in]  None
 * @return     void
 */
void init_lpuart(){	
	LPUART_config_baud57600(LPUART1);
	LPUART1->CTRL.RIE = 1;
	/*Enable transmitter and receiver*/
	LPUART1->CTRL.TE = 1;		/*Enable transmitter*/
	LPUART1->CTRL.RE = 1;		/*Enable receiver*/
	
	LPUART_config_baud57600(LPUART2);
	LPUART2->CTRL.RIE = 1;	
	/*Enable transmitter and receiver*/
	LPUART2->CTRL.TE = 1;		/*Enable transmitter*/
	LPUART2->CTRL.RE = 1;		/*Enable receiver*/
}

/*!
 * @brief     Initializes the user name data in flash memory.
 *
 * @detail    This function sets predefined names for users in the `name_user` array. It
 *            assigns specific characters to each element in the array, effectively
 *            initializing user names for the second, third, and fourth users. 
 *
 * @param[in]  None
 * @return     void
 */
void init_flash(){
	name_user[1][0] = 'M';
	name_user[1][1] = 'E';
	name_user[1][2] = 'N';
	
	name_user[2][0] = 'T';
	name_user[2][1] = 'H';
	name_user[2][2] = 'I';
	name_user[2][3] = 'N';
	name_user[2][4] = 'H';
	
	name_user[3][0] = 'V';
	name_user[3][1] = 'U';
}

/*!
 * @brief     Initializes the SysTick timer with the specified configuration.
 *
 * @detail    This function sets up the SysTick timer using a configuration structure. It
 *            specifies the clock source and interrupt mode for the SysTick timer. The
 *            `SysTick_Init` function is called with the provided configuration to apply
 *            the settings.
 *
 * @param[in]  None
 * @return     void
 */
void init_systick(){
	systick_config_t config = {
			.clk_source = 1,
			.interrupt_mode = 0,
	};
	SysTick_Init(config);
}

/*!
 * @brief     Checks if any button is pressed.
 *
 * @detail    This function sets all GPIOC pins high to prepare for reading the button state.
 *            It then reads the state of the columns to determine if any button is pressed.
 *            If any column reads as high, it returns 1 indicating a button is pressed.
 *            Otherwise, it returns 0.
 *
 * @param[in]  None
 * @return     1 if any button is pressed, 0 if no button is pressed.
 */
unsigned char check_but() { 
	GPIOC->PDOR |= ((1U << 8U) | (1U << 9U) | (1U << 10U) | (1U << 11U)); /* Set GPIOC pins high */
	unsigned char col0 = (GPIOC->PDIR >> 1U) & 0x01; /* Read column 0 */
	unsigned char col1 = (GPIOC->PDIR >> 2U) & 0x01; /* Read column 1 */
	unsigned char col2 = (GPIOC->PDIR >> 16U) & 0x01; /* Read column 2 */
	unsigned char col3 = (GPIOC->PDIR >> 15U) & 0x01; /* Read column 3 */
	
	if (col0 || col1 || col2 || col3) return 1; /* A button is pressed */
	return 0; /* No button is pressed */
}

/*!
 * @brief     Scans a specific row by setting the corresponding GPIOC pin high.
 *
 * @detail    This function clears all GPIOC pins and then sets the pin for the specified row high.
 *            It is used to detect button presses in a particular row.
 *
 * @param[in]  r The row number to scan (0-3).
 * @return     void
 */
void scan_row(unsigned char r){
	GPIOC->PDOR &= ~((1U << 8U)|(1U << 9U) | (1U << 10U) | (1U << 11U));
	if(r==0)   	GPIOC->PDOR |= (1U << 8U);
	else if(r==1)GPIOC->PDOR |= (1U << 9U);
	else if(r==2)GPIOC->PDOR |= (1U << 10U);
	else if(r==3)GPIOC->PDOR |= (1U << 11U);
}

/*!
 * @brief     Checks which column is active.
 *
 * @detail    This function reads the state of the columns to determine which one is active.
 *            It returns a value indicating the column number (1-4) that is active.
 *
 * @param[in]  None
 * @return     Column number (1-4) of the active column, 0 if no column is active.
 */
unsigned char check_col(){
	unsigned char c=0;
	unsigned char col0 = (GPIOC->PDIR >> 1U) & 0x01;
	unsigned char col1 = (GPIOC->PDIR >> 2U) & 0x01;
	unsigned char col2 = (GPIOC->PDIR >> 16U) & 0x01;
	unsigned char col3 = (GPIOC->PDIR >> 15U) & 0x01;
	if(col0)c=1; 
	else if(col1)c=2;
	else if(col2)c=3;
	else if(col3)c=4;
	return c; /* Get the column position */
}

/*!
 * @brief     Retrieves the position of the pressed key.
 *
 * @detail    This function determines which key is pressed by scanning each row and checking
 *            the active column. It returns the position of the pressed key in a 1-16 range.
 *            If no key is pressed, it returns 0.
 *
 * @param[in]  None
 * @return     The position of the pressed key (1-16), or 0 if no key is pressed.
 */
unsigned char get_key() { 
	unsigned char row, col;
	if (check_but()) { 
		for (row = 0; row < 4; row++) {
			scan_row(row); 
			col = check_col(); 
			if (col > 0) return ((row * 4) + col); 
		}
	}
	return 0; 
}

/*!
 * @brief     Performs fingerprint search operation.
 *
 * @detail    This function coordinates the process of searching for a fingerprint by:
 *            1. Receiving a fingerprint image from the user.
 *            2. Generating a feature file for the fingerprint.
 *            3. Sending a search instruction and displaying the result.
 *
 * @param[in]  None
 * @return     void
 */
void search_finger_print(){
	/* Step 1: Receive a fingerprint */
	do{
		LPUART_send_string(LPUART1, (unsigned char*)"Press your finger to search");
		lcd_clear();
		lcd_put_cur(0, 0);
		lcd_send_string("PRESS FINGER");
		display_time();
		do{	
			LPUART_send_string(LPUART1, (unsigned char*)".");
			display_time();
			GPIO_SetOutputPin(GPIOD, 1);
		}while(sendFPGetImage(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
		LPUART_send_byte(LPUART1, 0x0A);
		
		/* Step 2: Generate features file */
		LPUART_send_string(LPUART1, (unsigned char*)"Receiving your finger image ");
		display_time();
		LPUART_send_byte(LPUART1, 0x0A);
	}while(sendFPCreateCharFile1(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
	LPUART_send_string(LPUART1, (unsigned char*)"Received your finger image");
	display_time();
	LPUART_send_byte(LPUART1, 0x0A);
	
	/* Step 3: Send Search instruction data */
	LPUART_send_string(LPUART1, (unsigned char*)"Searching finger");
	display_time();
	GPIO_SetOutputPin(GPIOD, 1);
	lcd_clear();
	lcd_put_cur(0, 0);
	lcd_send_string("SEARCHING");
	LPUART_send_byte(LPUART1, 0x0A);
	if(sendFPSearchFinger(LPUART2, data_ack, CORE_CLOCK) == FINGERPRINT_OK){
		lcd_clear();
		lcd_put_cur(0, 0);
		lcd_send_string(name_user[data_ack[6]]);
		GPIO_ResetOutputPin(GPIOD, 1);
		display_time();
		delay(1000, CORE_CLOCK);
	}else{
		lcd_clear();
		lcd_put_cur(0, 0);
		GPIO_SetOutputPin(GPIOD, 1);
		lcd_send_string("NOT FOUND");
		display_time();
		delay(1000, CORE_CLOCK);		
	}
}

/*!
 * @brief     Imports a fingerprint and stores it in the system.
 *
 * @detail    This function performs the following steps:
 *            1. Prompts the user to provide an ID for the fingerprint.
 *            2. Receives a fingerprint image and creates feature files.
 *            3. Receives the same fingerprint again to generate a template.
 *            4. Saves the fingerprint template to flash memory.
 *            5. Switches to name creation mode.
 *
 * @param[in]  None
 * @return     void
 */
void import_finger_print(){
	LPUART_send_string(LPUART1, (unsigned char*)"Give ID to store finger");
	LPUART_send_byte(LPUART1, 0x0A);
	while(IDStore == 0){} /*Waiting receive data from uart*/
	do{
		
		/* Step 1: Receive a fingerprint */
		do{
			LPUART_send_string(LPUART1, (unsigned char*)"Press your finger");
			do{	
				LPUART_send_string(LPUART1, (unsigned char*)".");
			}while(sendFPGetImage(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
			LPUART_send_byte(LPUART1, 0x0A);
			
			/* Step 2: Generate features file 1*/
			LPUART_send_string(LPUART1, (unsigned char*)"Creating Char File 1");
			LPUART_send_byte(LPUART1, 0x0A);
		}while(sendFPCreateCharFile1(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
		LPUART_send_string(LPUART1, (unsigned char*)"Created CHAR_FILE_1");
		LPUART_send_byte(LPUART1, 0x0A);
		while(sendFPGetImage(LPUART2, data_ack, CORE_CLOCK) == FINGERPRINT_OK){
			LPUART_send_string(LPUART1, (unsigned char*)"Remove your finger");
			LPUART_send_byte(LPUART1, 0x0A);
		}
		/* Step 3: Receive a same fingerprint */
		do{
			LPUART_send_string(LPUART1, (unsigned char*)"Press your finger again (1)");
			do{	
				LPUART_send_string(LPUART1, (unsigned char*)".");
			}while(sendFPGetImage(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
			LPUART_send_byte(LPUART1, 0x0A);
			
			/* Step 4: Generate features file 2*/
			LPUART_send_string(LPUART1, (unsigned char*)"Creating Char File 2");
			LPUART_send_byte(LPUART1, 0x0A);
		}while(sendFPCreateCharFile2(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
		LPUART_send_string(LPUART1, (unsigned char*)"Created CHAR_FILE_2");
		LPUART_send_byte(LPUART1, 0x0A);
		while(sendFPGetImage(LPUART2, data_ack, CORE_CLOCK) == FINGERPRINT_OK){
			LPUART_send_string(LPUART1, (unsigned char*)"Remove your finger");
			LPUART_send_byte(LPUART1, 0x0A);
		}
		/* Step 5: Compare char file 1 and char file 2 to generate the template file */
		LPUART_send_string(LPUART1, (unsigned char*)"Creating template model");
		LPUART_send_byte(LPUART1, 0x0A);
	}while(sendFPCreateTemplate(LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
	LPUART_send_string(LPUART1, (unsigned char*)"Created TEMPLATE MODEL");
	LPUART_send_byte(LPUART1, 0x0A);
	
	/* Step 6: Save the template file to AS608 FLASH */
	do {
		LPUART_send_string(LPUART1, (unsigned char*)"Storing");
		LPUART_send_byte(LPUART1, 0x0A);
	}while(SendStoreFinger(IDStore, LPUART2, data_ack, CORE_CLOCK) != FINGERPRINT_OK);
	LPUART_send_string(LPUART1, (unsigned char*)"Storaged");
	LPUART_send_byte(LPUART1, 0x0A);
	/* Step 7: Switched to name creation mode */
	finger_mode = CREATE_NEW_USER_NAME_MODE;		
}

/*!
 * @brief     Handles the number input mode for setting a name or performing actions.
 *
 * @detail    This function processes user inputs for setting a name, performing operations,
 *            and interacting with the fingerprint module based on the input value.
 *            
 *            Actions performed based on input values:
 *            - 0: No action.
 *            - 1-3: Add numbers 1-3 to the current position in the user name.
 *            - 4: Switch to a different mode (MODE = 1).
 *            - 5-7: Add numbers 4-6 to the current position in the user name.
 *            - 8: Add a space to the current position in the user name.
 *            - 9-11: Add numbers 7-9 to the current position in the user name.
 *            - 12: Clear the user name and reset cursor position.
 *            - 13: Clear all stored fingerprints.
 *            - 14: Add number 0 to the current position in the user name.
 *            - 15: Finalize name creation, switch to fingerprint search mode.
 *            - 16: Remove the last character from the user name.
 *
 * @param[in]  None
 * @return     void
 */
void keytap_number_mode(){
	switch (value) {
	case 0:
		break;
	case 1:
		lcd_send_string("1");
		name_user[IDStore][cursor_position] = '1';
		cursor_position++;
		break;
	case 2:
		lcd_send_string("2");
		name_user[IDStore][cursor_position] = '2';
		cursor_position++;
		break;
	case 3:
		lcd_send_string("3");
		name_user[IDStore][cursor_position] = '3';
		cursor_position++;
		break;
	case 4:
		MODE = 1;
		break;
	case 5:
		lcd_send_string("4");
		name_user[IDStore][cursor_position] = '4';
		cursor_position++;
		break;
	case 6:
		lcd_send_string("5");
		name_user[IDStore][cursor_position] = '5';
		cursor_position++;
		break;
	case 7:
		lcd_send_string("6");
		name_user[IDStore][cursor_position] = '6';
		cursor_position++;
		break;
	case 8:
		lcd_send_string(" ");
		name_user[IDStore][cursor_position] = ' ';
		cursor_position++;
		break;
	case 9:
		lcd_send_string("7");
		name_user[IDStore][cursor_position] = '7';
		cursor_position++;
		break;
	case 10:
		lcd_send_string("8");
		name_user[IDStore][cursor_position] = '8';
		cursor_position++;
		break;
	case 11:
		lcd_send_string("9");
		name_user[IDStore][cursor_position] = '9';
		cursor_position++;
		break;
	case 12:
		lcd_clear();
		cursor_position = 0;
		for(unsigned char i = 0; i < MAX_NAME_LENGTH; i++){
			name_user[IDStore][i] = '\0';
		}
		lcd_put_cur(0,0);
		break;
	case 13:
		if(sendFPDeleteAllFinger(LPUART2, data_ack, CORE_CLOCK) == FINGERPRINT_OK){
			lcd_clear();
			lcd_put_cur(0, 0);
			lcd_send_string("CLEAR ALL FINGER");
			delay(1000, CORE_CLOCK);
			lcd_clear();
			lcd_put_cur(0, 0);
		}
		break;
	case 14:
		lcd_send_string("0");
		name_user[IDStore][cursor_position] = '0';
		cursor_position++;
		break;
	case 15:
		IDStore = 0;
		lcd_clear();
		lcd_put_cur(0, 0);
		lcd_send_string("CREATED NAME");
		delay(1000, CORE_CLOCK);
		lcd_clear();
		lcd_put_cur(0, 0);
		finger_mode = SEARCH_FINGERPRINT_MODE;
		break;
	case 16:
		cursor_position--;
		name_user[IDStore][cursor_position] = '\0';
		lcd_send_cmd(0x80 | cursor_position);
		lcd_send_string(" "); 
		lcd_send_cmd(0x80 | cursor_position);
		break;
	default:
		// Handle unexpected values
		lcd_send_string("Invalid");
		break;
	}
}

/*!
 * @brief     Handles character input mode for setting a name.
 *
 * @detail    This function processes user inputs for setting a name based on the button press
 *            count. It maps button press counts to specific characters and updates the display
 *            and user name accordingly.
 *            
 *            Actions performed based on input values:
 *            - 0: No action.
 *            - 1: Add characters A, B, or C based on button press count.
 *            - 2: Add characters C or D based on button press count.
 *            - 3: Add characters E or G based on button press count.
 *            - 4: Switch to number mode (MODE = 0).
 *            - 5: Add characters H or I based on button press count.
 *            - 6: Add characters K or L based on button press count.
 *            - 7: Add characters M or N based on button press count.
 *            - 8: Add a space to the current position in the user name.
 *            - 9: Add characters O or P based on button press count.
 *            - 10: Add characters Q or R based on button press count.
 *            - 11: Add characters S or T based on button press count.
 *            - 12: Clear the user name and reset cursor position.
 *            - 13: Add characters U or V based on button press count.
 *            - 14: Add characters X or Y based on button press count.
 *            - 15: Finalize name creation, switch to fingerprint search mode.
 *            - 16: Remove the last character from the user name.
 *
 * @param[in]  None
 * @return     void
 */
void keytap_character_mode(){
	switch (value) {
	case 0:
			break;
	case 1:
		button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1){
			lcd_send_string("A");
			name_user[IDStore][cursor_position] = 'A';
		}
		else if (button_press_count == 2) {
			lcd_send_string("B");
			name_user[IDStore][cursor_position] = 'B';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 2:
		 button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("C");
			name_user[IDStore][cursor_position] = 'C';
		}
		else if (button_press_count == 2) {
			lcd_send_string("D");
			name_user[IDStore][cursor_position] = 'D';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 3:
		button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("E");
			name_user[IDStore][cursor_position] = 'E';
		}
		else if (button_press_count == 2) {
			lcd_send_string("G");
			name_user[IDStore][cursor_position] = 'G';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 4:
			MODE = 0;
			break;
	case 5:
	 button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("H");
			name_user[IDStore][cursor_position] = 'H';
		}
		else if (button_press_count == 2) {
			lcd_send_string("I");
			name_user[IDStore][cursor_position] = 'I';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 6:
	 button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("K");
			name_user[IDStore][cursor_position] = 'K';
		}
		else if (button_press_count == 2) {
			lcd_send_string("L");
			name_user[IDStore][cursor_position] = 'L';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 7:
		button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1){
			lcd_send_string("M");
			name_user[IDStore][cursor_position] = 'M';
		}
		else if (button_press_count == 2) {
			lcd_send_string("N");
			name_user[IDStore][cursor_position] = 'N';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 8:
		lcd_send_string(" ");
		name_user[IDStore][cursor_position] = ' ';
		cursor_position++;
		break;
	case 9:
		 button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("O");
			name_user[IDStore][cursor_position] = 'O';
		}
		else if (button_press_count == 2) {
			name_user[IDStore][cursor_position] = 'P';
			lcd_send_string("P");
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 10:
		button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("Q");
			name_user[IDStore][cursor_position] = 'Q';
		}
		else if (button_press_count == 2){ 
			lcd_send_string("R");
			name_user[IDStore][cursor_position] = 'R';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 11:
		 button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("S");
			name_user[IDStore][cursor_position] = 'S';
		}
		else if (button_press_count == 2) {
			lcd_send_string("T");
			name_user[IDStore][cursor_position] = 'T';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 12:
		lcd_clear();
		cursor_position = 0;
		for(unsigned char i = 0; i < MAX_NAME_LENGTH; i++){
			name_user[IDStore][i] = '\0';
		}
		lcd_put_cur(0,0);
		break;
	case 13:
	button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("U");
			name_user[IDStore][cursor_position] = 'U';
		}
		else if (button_press_count == 2) {
			lcd_send_string("V");
			name_user[IDStore][cursor_position] = 'V';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 14:
			button_press_count++;
		delay(500, CORE_CLOCK);
		if (check_but() == 1){	
					button_press_count++;
		}
		if (button_press_count == 1) {
			lcd_send_string("X");
			name_user[IDStore][cursor_position] = 'X';
		}
		else if (button_press_count == 2) {
			lcd_send_string("Y");
			name_user[IDStore][cursor_position] = 'Y';
		}
		button_press_count = 0;
		cursor_position++;
		break;
	case 15:
		IDStore = 0;
		lcd_clear();
		lcd_put_cur(0, 0);
		lcd_send_string("CREATED NAME");
		delay(1000, CORE_CLOCK);
		lcd_clear();
		lcd_put_cur(0, 0);
		finger_mode = SEARCH_FINGERPRINT_MODE;
		break;
	case 16:
		cursor_position--;
		name_user[IDStore][cursor_position] = '\0';
		lcd_send_cmd(0x80 | cursor_position);
		lcd_send_string(" "); 
		lcd_send_cmd(0x80 | cursor_position);
		break;
	}
}

/*!
 * @brief     Handles keytap input based on the current mode (number or character).
 *
 * @detail    This function checks the current mode to determine if the system is in
 *            number or character input mode. It updates the state of an output pin
 *            (GPIOD, pin 15) based on the mode, processes the keypress, and updates
 *            the display and user name as needed. If the cursor position exceeds a
 *            certain limit, it clears the display and resets the user name.
 *            
 *            - If in number mode, sets an output pin high.
 *            - If in character mode, sets the output pin low.
 *            - Calls the appropriate function to handle key input based on the mode.
 *            - Clears the LCD and resets the user name if cursor position exceeds 16.
 *            
 * @param[in]  None
 * @return     void
 */
void handle_keytap(){
	if(MODE == KEYTAP_NUMBER_MODE){
		GPIO_SetOutputPin(GPIOD, 15);
	}	
	else {
		GPIO_ResetOutputPin(GPIOD, 15);
	}
	value = get_key();
	if (MODE == KEYTAP_NUMBER_MODE){
		keytap_number_mode();
	}
	else if(MODE == KEYTAP_CHARACTER_MODE) {
		keytap_character_mode();
	}
	if(cursor_position > 16)
	{
		lcd_clear();
		cursor_position = 0;
		lcd_put_cur(0,0);
		for(unsigned char i = 0; i < MAX_NAME_LENGTH; i++){
			name_user[IDStore][i] = '\0';
		}
	}
}

/**********************************************************************************************
 * @brief    Configures port C6 (PTC6) as LPUART1_RX (Receive).
 * 
 * @detail   This function sets up port C6 for LPUART1 reception. The configuration includes
 *           setting the multiplexer to select the LPUART1 functionality and disabling pull-up
 *           and pull-down resistors. This is used for debugging purposes with LPUART1.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 2 (LPUART1_RX function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configC6(){
	Port_Mode_t config_PORT_C6 = {
		.IQRC = 0,
		.MUX = 2,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTC, 6, config_PORT_C6);
}

/**********************************************************************************************
 * @brief    Configures port C7 (PTC7) as LPUART1_TX (Transmit).
 * 
 * @detail   This function sets up port C7 for LPUART1 transmission. The configuration includes
 *           setting the multiplexer to select the LPUART1 functionality and disabling pull-up
 *           and pull-down resistors. This is used for debugging purposes with LPUART1.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 2 (LPUART1_TX function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configC7(){
	Port_Mode_t config_PORT_C7 = {
		.IQRC = 0,
		.MUX = 2,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTC, 7, config_PORT_C7);
}

/**********************************************************************************************
 * @brief    Configures port A2 (PTA2) as LPI2C_SDA (Data Line).
 * 
 * @detail   This function sets up port A2 for LPI2C data line (SDA). The configuration includes
 *           setting the multiplexer to select the LPI2C functionality and disabling pull-up
 *           and pull-down resistors. This is used for communication with the LCD1602.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 3 (LPI2C_SDA function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configA2(){
	Port_Mode_t config_PORT = {
		.IQRC = 0,
		.MUX = 3,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTA, 2, config_PORT);
}

/**********************************************************************************************
 * @brief    Configures port A3 (PTA3) as LPI2C_SCL (Clock Line).
 * 
 * @detail   This function sets up port A3 for LPI2C clock line (SCL). The configuration includes
 *           setting the multiplexer to select the LPI2C functionality and disabling pull-up
 *           and pull-down resistors. This is used for communication with the LCD1602.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 3 (LPI2C_SCL function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configA3(){
	Port_Mode_t config_PORT = {
		.IQRC = 0,
		.MUX = 3,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTA, 3, config_PORT);
}

/**********************************************************************************************
 * @brief    Configures port D7 (PTD7) as LPUART2_TX (Transmit Line).
 * 
 * @detail   This function sets up port D7 for LPUART2 transmit line (TX). The configuration includes
 *           setting the multiplexer to select the LPUART2 functionality and disabling pull-up
 *           and pull-down resistors. This port is used for communication with the AS608 fingerprint
 *           sensor.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 2 (LPUART2_TX function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configD7(){
	Port_Mode_t config_PORT = {
		.IQRC = 0,
		.MUX = 2,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTD, 7, config_PORT);
}

/**********************************************************************************************
 * @brief    Configures port D6 (PTD6) as LPUART2_RX (Receive Line).
 * 
 * @detail   This function sets up port D6 for LPUART2 receive line (RX). The configuration includes
 *           setting the multiplexer to select the LPUART2 functionality and disabling pull-up
 *           and pull-down resistors. This port is used for communication with the AS608 fingerprint
 *           sensor.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 2 (LPUART2_RX function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configD6(){
	Port_Mode_t config_PORT = {
		.IQRC = 0,
		.MUX = 2,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTD, 6, config_PORT);
}

/**********************************************************************************************
 * @brief    Configures port D15 (PTD15) for indicating the mode of the keytap.
 * 
 * @detail   This function sets up port D15 for general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality and disabling pull-up and pull-down
 *           resistors. This port is used to indicate the mode of the keytap operation (e.g., LED indicator).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configD15(){
	Port_Mode_t config_PORT = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTD, 15, config_PORT);
	
	Gpio_ConfigType config_GPIO = {
		.base = GPIOD,
		.GPIO_PinMode = 1,
		.GPIO_PinNumber = 15,
	};
	Gpio_Init(&config_GPIO);
}

/**********************************************************************************************
 * @brief    Configures port D1 (PTD1) for connection to the ESP32, configured as a GPIO output.
 * 
 * @detail   This function sets up port D1 for general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality and disabling pull-up and pull-down
 *           resistors. This port is used to interface with the ESP32.
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void configD1(){
	Port_Mode_t config_PORT = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Port_Init(PORTD, 1, config_PORT);
	
	Gpio_ConfigType config_GPIO = {
		.base = GPIOD,
		.GPIO_PinMode = 1,
		.GPIO_PinNumber = 1,
	};
	Gpio_Init(&config_GPIO);
}

/**********************************************************************************************
 * @brief    Configures port C8 (PTC8) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C8 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, disabling pull-up and pull-down
 *           resistors, and setting the pin as an output (assuming it will drive a row in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC8(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 1,
		.GPIO_PinNumber = 8,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 8, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C9 (PTC9) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C9 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, disabling pull-up and pull-down
 *           resistors, and setting the pin as an output (assuming it will drive a row in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC9(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 1,
		.GPIO_PinNumber = 9,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 9, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C10 (PTC10) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C10 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, disabling pull-up and pull-down
 *           resistors, and setting the pin as an output (assuming it will drive a row in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC10(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 1,
		.GPIO_PinNumber = 10,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 10, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C11 (PTC11) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C11 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, disabling pull-up and pull-down
 *           resistors, and setting the pin as an output (assuming it will drive a row in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 0 (no pull-up or pull-down resistors).
 *           - `PullUpDown` is set to 0 (no pull-up or pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC11(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 0,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 1,
		.GPIO_PinNumber = 11,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 11, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C1 (PTC1) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C1 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, enabling pull-up resistors, and setting
 *           the pin as an input (assuming it will read a column in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 1 (enable pull-up resistor).
 *           - `PullUpDown` is set to 0 (no pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC1(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 1,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 0,
		.GPIO_PinNumber = 1,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 1, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C2 (PTC2) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C2 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, enabling pull-up resistors, and setting
 *           the pin as an input (assuming it will read a column in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 1 (enable pull-up resistor).
 *           - `PullUpDown` is set to 0 (no pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC2(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 1,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 0,
		.GPIO_PinNumber = 2,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 2, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C16 (PTC16) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C16 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, enabling pull-up resistors, and setting
 *           the pin as an input (assuming it will read a column in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 1 (enable pull-up resistor).
 *           - `PullUpDown` is set to 0 (no pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC16(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 1,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 0,
		.GPIO_PinNumber = 16,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 16, config_Port);
}

/**********************************************************************************************
 * @brief    Configures port C15 (PTC15) for communication with the 4x4 matrix button.
 * 
 * @detail   This function sets up port C15 as a general-purpose input/output (GPIO). The configuration
 *           includes setting the multiplexer to GPIO functionality, enabling pull-up resistors, and setting
 *           the pin as an input (assuming it will read a column in the matrix).
 *           
 *           - `IQRC` is set to 0 (default setting).
 *           - `MUX` is set to 1 (GPIO function).
 *           - `PullEnable` is set to 1 (enable pull-up resistor).
 *           - `PullUpDown` is set to 0 (no pull-down resistors).
 *
 * @param     None
 * @return    void
 */
void config_PTC15(){
	Port_Mode_t config_Port = {
		.IQRC = 0,
		.MUX = 1,
		.PullEnable = 1,
		.PullUpDown = 0,
	};
	Gpio_ConfigType config_GPIO = {
		.base = GPIOC,
		.GPIO_PinMode = 0,
		.GPIO_PinNumber = 15,
	};
	Gpio_Init(&config_GPIO);
	Port_Init(PORTC, 15, config_Port);

}

