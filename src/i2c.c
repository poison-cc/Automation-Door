/**
*   @file    i2c.c
*   @brief   Implementation of I2C communication functions and LCD control.
*   @details This file includes functions to initialize the LPI2C0 peripheral, 
*            transmit and receive data via I2C, and control an LCD display through 
*            I2C communication. It also handles error detection for common I2C 
*            issues such as bus busy, no data received, and stop condition errors. 
*            Additionally, functions for sending commands, data, and managing the 
*            LCD cursor and display are provided.
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "i2c.h"
#include "systick.h"

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* Enum for error codes */
enum err
{
    OK,                  /* No error */
    BUSY,                /* I2C bus is busy */
    NO_DATA_RECEIVED,    /* No data received */
    NO_STOP,             /* No stop condition detected */
    NDF,                 /* NACK detected */
    ALF,                 /* Arbitration lost */
    FEF,                 /* FIFO error */
    PLTF                 /* Pin low timeout */
};

/* Timeouts for various I2C operations in milliseconds */
#define BUSY_TIMEOUT        1000  /* Timeout for detecting bus busy */
#define READING_TIMEOUT     2000  /* Timeout for reading data */
#define STOP_TIMEOUT        3000  /* Timeout for detecting stop condition */

/* I2C address of the LCD display */
#define SLAVE_ADDRESS_LCD 0x4E 

/*==================================================================================================
*                                       STATIC VARIABLES
==================================================================================================*/
static unsigned char error = 0;

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*******************************************************************************
Function Name : LPI2C0_init
Notes         : BAUD RATE: 400 kbps
                I2C module frequency 8Mhz (SIRCDIV2_CLK)
                PRESCALER:0x00; FILTSCL/SDA:0x0/0x0; SETHOLD:0x4; CLKLO:0x0B; CLKHI:0x05; DATAVD:0x02
                See Table 50-10 Example timing configuration in S32K1xx Reference manual rev.9
 *******************************************************************************/
void init_LPI2C0(void)
{
	   /* Disable module for configuration */
    LPI2C0->MCR_REGISTER = 0x00000000;
    LPI2C0->MCCR0_REGISTER = 0x0204050B;
	
    // [24] DATAVD  0x02
    // [16] SETHOLD 0x04
    // [8]  CLKHI   0x05
    // [0]  CLKLO   0x0B
	
	  LPI2C0->MCCR1_REGISTER = 0x0204050B;

    // [24] DATAVD  0x02
    // [16] SETHOLD 0x04
    // [8]  CLKHI   0x05
    // [0]  CLKLO   0x0B

    // Master Interrupt Enable Register (MIER)
    LPI2C0->MIER_REGISTER = 0x0002;
    // [14] DMIE = 0  (Data match interrupt disabled)
    // [13] PLTIE = 1 (Pin low timeout interrupt enabled)
    // [12] FEIE = 1  (FIFO error interrupt enabled)
    // [11] ALIE = 1  (Arbitration lost interrupt enabled)
    // [10] NDIE = 1  (NACK detect interrupt enabled)
    // [9]  SDIE = 0  (STOP detect interrupt disabled)
    // [8]  EPIE = 0  (End packet interrupt disabled)

    // Master Configuration Register 0
    LPI2C0->MCFGR0_REGISTER = 0x0000;
    // [9] RDMO = 0    (Received data is stored in the receive FIFO as normal)
    // [8] CIRFIFO = 0 (Circular FIFO is disabled)
    // [2] HRSEL = 0   (Host request input is pin HREQ)
    // [1] HRPOL = 0   (Active low)
    // [0] HREN = 0    (Host request input is disabled)

    // Master Configuration Register 1
    LPI2C0->MCFGR1_REGISTER = 0x00000800;
    // [26-14] PINCFG     = 0b000 (LPI2C configured for 2-pin open drain mode)
    // [16-16] MATCFG     = 0b000 (Match disabled)
    // [10]    TIMECFG    = 1     (Pin Low Timeout Flag will set if either SCL or SDA is low for longer than the configured timeout)
    // [9]     IGNACK     = 0     (LPI2C Master will receive ACK and NACK normally)
    // [8]     AUTOSTOP   = 0     (Without autostop generation)
    // [2-0]   PRESCALE   = 0b000 (Divided by 1)

    // Master Configuration Register 2
    LPI2C0->MCFGR2_REGISTER = 0x0000001F;
    // [27-24] FILTSDA = 0x0  (Disable the glitch filter)
    // [19-16] FILTSDA = 0x0  (Disable the glitch filter)
    // [11-0]  BUSIDLE = 0x1F (Bus idle timeout period in clock cycles)

    // Master Configuration Register 3
    LPI2C0->MCFGR3_REGISTER = 0x00000200;
    // [19-8] PINLOW = 0x0002 (Pin Low Timeout enabled)

    // Master FIFO Control Register
    LPI2C0->MFCR_REGISTER = 0x00000000;
    // [17-16] RXWATER = 0 (Receive FIFO watermark)
    // [1-0]   TXWATER = 0 (Transmit FIFO watermark)

    // Master Control Register
    LPI2C0->MCR_REGISTER	 = 0x901;
    // [9] RRF = 1   (Receive FIFO is reset)
    // [8] RTF = 1   (Transmit FIFO is reset)
    // [3] DBGEN = 0 (Master is disabled in debug mode)
    // [2] DOZEN = 0 (Master is disabled in Doze mode)
    // [1] RST = 0   (Master logic is not reset)
    // [0] MEN = 1   (Master logic is enabled)
}

/**
* @brief    Checks if the I2C bus is busy.
* @return   Returns 0 (OK) if the bus is free; otherwise, returns an error code.
* @details  Continuously checks the Bus Busy Flag (BBF) in the status register 
*           to determine if the I2C bus is busy. Times out if the bus remains 
*           busy for too long.
*/
unsigned char bus_busy(void)
{
     error = 0;                 // CLEAR ALL ERRORS

     unsigned short int timeout_b = 0;
     while ((LPI2C0->MSR.BBF) && (timeout_b < BUSY_TIMEOUT))  ++timeout_b;

     if(timeout_b >= BUSY_TIMEOUT) return (error |= (1 << BUSY));

     /*
      * For debugging purposes
      */ //PTD-> PCOR |= (1 << 0); // BLUE LED ON

     return OK;
}

/*******************************************************************************
Function Name : I2C_start_ACK
Parameters    : uint8_t address
Notes         : Generate (repeated) START and transmit address in DATA[7:0]
 *******************************************************************************/
void generate_start_ACK(unsigned char address)
{
    LPI2C0->MTDR_REGISTER =  (unsigned int)((4U << 8U) | (address & 0xFF));
}

/**
* @brief    Transmits a data byte over I2C.
* @param    data: The byte of data to be sent.
* @details  Loads the data to be transmitted into the MTDR register. The data is 
*           then sent over the I2C bus to the target device.
*/
void transmit_data(unsigned char data)
{
	//while(!LPI2C0->MSR.TDF);
    LPI2C0->MTDR_REGISTER = (unsigned int)((0U << 8U) | (data & 0xFF));
}

/**
* @brief    Generates a STOP condition on the I2C bus.
* @return   Returns 0 (OK) if the STOP condition was successfully sent; 
*           otherwise, returns an error code.
* @details  Sends a STOP condition to terminate communication with the I2C slave 
*           device. Monitors the Stop Detect Flag (SDF) to ensure the STOP 
*           condition was recognized.
*/
unsigned char generate_stop(void)
{
    unsigned int timeout_s      = 0;
    unsigned char stop_sent_err   = 0;

    LPI2C0->MTDR_REGISTER = 0x0200; //command

    while((!(LPI2C0->MSR.SDF)) && (!stop_sent_err))
    {
        if(timeout_s > STOP_TIMEOUT)
        {
            error |= (1 << NO_STOP);
            stop_sent_err = 1;
        }
        timeout_s++;
    }

    if(LPI2C0->MSR.SDF)
    {
        LPI2C0->MSR.SDF = 1; // Clear Stop flag
    }

     if(error) return error;
     else return OK;
}

/**
* @brief    Writes a byte of data to a specific register on the I2C slave device.
* @param    s_w_address: The 7-bit I2C address of the slave device.
* @param    s_reg_address: The address of the register on the slave device to write to.
* @param    byte: The data byte to be written.
* @return   Returns 0 (OK) if the write operation was successful; otherwise, returns an error code.
* @details  Sends a START condition followed by the slave address and register address.
*           Writes the specified data byte to the slave device and then generates a STOP condition.
*/
unsigned char LPI2C0_write(unsigned char s_w_address, unsigned char s_reg_address, unsigned char byte)
{	
    if(bus_busy()) return (error |= (1 << BUSY));
    generate_start_ACK(s_w_address);
    transmit_data(s_reg_address);
    transmit_data(byte);
    if(generate_stop()) return error;
    else return OK;
}

/**
* @brief    Reads a byte of data from a specific register on the I2C slave device.
* @param    s_w_address: The 7-bit I2C address of the slave device.
* @param    s_reg_address: The address of the register on the slave device to read from.
* @return   Returns the data byte read from the specified register on the slave device.
* @details  Sends a START condition followed by the slave address and register address.
*           Generates a repeated START condition to switch to read mode and reads the data 
*           byte from the specified register on the slave device.
*/
unsigned char LPI2C0_read(unsigned char s_w_address, unsigned char s_reg_address)
{	
    unsigned char s_r_address = s_w_address | 0x01; // Set the read bit

    if(bus_busy()) return (error |= (1 << BUSY));
    
    // Start and send slave write address
    generate_start_ACK(s_w_address);
    transmit_data(s_reg_address);
    
    // Repeated start and send slave read address
    generate_start_ACK(s_r_address);

    if(generate_stop()) return error;
    else return OK;
}

/**
* @brief    Sends a command to the LCD over I2C.
* @param    cmd: The command byte to be sent to the LCD.
* @details  Splits the command byte into two 4-bit nibbles and sends each nibble 
*           to the LCD, along with the necessary control signals.
*/
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	unsigned char data_t[4];
	data_u = (cmd&0xf0);
	
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0

	for(int i = 0; i < 4; i++)
	{
		LPI2C0_write(SLAVE_ADDRESS_LCD, 0x00, data_t[i]);
	}
}

/**
* @brief    Sends data to the LCD over I2C.
* @param    data: The data byte to be sent to the LCD.
* @details  Splits the data byte into two 4-bit nibbles and sends each nibble 
*           to the LCD, along with the necessary control signals.
*/
void lcd_send_data(char data)
{
	char data_u, data_l;
	unsigned char data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0

	for(int i = 0; i < 4; i++)
	{
		LPI2C0_write(SLAVE_ADDRESS_LCD, 0x00, data_t[i]);
	}
}

/**
* @brief    Positions the LCD cursor to a specific location.
* @param    row: The row number (0 or 1) where the cursor should be placed.
* @param    col: The column number (0-15) where the cursor should be placed.
* @details  Sends a command to the LCD to move the cursor to the specified row and column.
*/
void lcd_put_cur(unsigned char row, unsigned char col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd(col);
}

/**
* @brief    Clears the entire LCD display.
* @details  Sends the appropriate command to clear the LCD display. Fills the display with 
*           blank spaces to ensure it is fully cleared.
*/
void lcd_clear (void) 
{
	unsigned char i;
	lcd_send_cmd (0x80);
	for (i=0; i<16; i++)
	{
		lcd_send_data (' ');
	}
}

/**
* @brief    Clears the portion of the LCD display showing the RTC (Real-Time Clock) data.
* @details  Clears a specific area of the LCD display where the RTC data is shown, 
*           replacing it with blank spaces.
*/
void lcd_clear_rtc (void) 
{
	unsigned char i;
	lcd_send_cmd (0xC7);
	for (i=0; i<8; i++)
	{
		lcd_send_data (' ');
	}
}

/**
* @brief    Initializes the LCD for 4-bit communication.
* @details  Sends a sequence of commands to initialize the LCD in 4-bit mode and set up 
*           the display parameters, including cursor increment and display on/off settings.
*/
void lcd_init (void)
{
	/* 4 bit initialisation*/
	delay(50, 80000000); /* wait for >40ms*/
	lcd_send_cmd (0x30);
	delay(5, 80000000);  /* wait for >4.1ms*/
	lcd_send_cmd (0x30);
	delay(1, 80000000);  /* wait for >100us*/
	lcd_send_cmd (0x30);
	delay(10, 80000000);
	lcd_send_cmd (0x20); /* 4bit mode*/
	delay(10, 80000000);

  // dislay initialisation
	lcd_send_cmd (0x28); /* Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)*/
	delay(1, 80000000);
	lcd_send_cmd (0x08); /*Display on/off control --> D=0,C=0, B=0  ---> display off*/
	delay(1, 80000000);
	lcd_send_cmd (0x01);  /* clear display*/
	delay(2, 80000000);
	lcd_send_cmd (0x06); /*Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)*/
	delay(1, 80000000);
	lcd_send_cmd (0x0C); /*Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)*/
}

/**
* @brief    Sends a string of characters to the LCD.
* @param    str: Pointer to the null-terminated string to be sent to the LCD.
* @details  Sends each character in the string to the LCD one by one, until the null terminator is reached.
*/
void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}



