/**
*   @file    lpuart_register.h
*   @brief   Definition of Register for LPUART module
*   @details This file contains the structure definitions for the LPUART registers, 
*            including base addresses for different LPUART instances. It also ensures 
*            security to avoid multiple declarations by using include guards.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef LPUART_REGISTER_H_
#define LPUART_REGISTER_H_

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          struct name LPUART_t
* @details        This structure represents the LPUART registers and their respective offsets.
*
*/
typedef struct {
	struct {
		volatile const unsigned int FEATURE: 16;
		volatile const unsigned int MINOR: 8;
		volatile const unsigned int MAJOR:8;	
	}VERID;
	struct {
		volatile const unsigned int TXFIFO: 8;
		volatile const unsigned int RXFIFO: 8;
		volatile const unsigned int RESERVED: 16;	
	}PARAM;
	struct {
		volatile const unsigned int RESERVED1: 1;
		volatile unsigned int RST: 1;
		volatile const unsigned int RESERVED2: 14;
		volatile const unsigned int RESERVED3: 16;	
	}GLOBAL;
	struct {
		volatile unsigned int TRGSEL: 2;
		volatile const unsigned int RESERVED: 30;	
	}PINCFG;
	struct {
		volatile unsigned int SBR: 13;
		volatile unsigned int SBNS: 1;
		volatile unsigned int RXEDGIE: 1;
		volatile unsigned int LBKDIE: 1;
		volatile unsigned int RESYNCDIS: 1;
		volatile unsigned int BOTHEDGE: 1;
		volatile unsigned int MATCFG: 2;
		volatile unsigned int RIDMAE: 1;
		volatile unsigned int RDMAE: 1;
		volatile unsigned int RESERVED1: 1;
		volatile unsigned int TDMAE: 1;
		volatile unsigned int OSR: 5;
		volatile unsigned int M10: 1;
		volatile unsigned int MAEN2: 1;
		volatile unsigned int MAEN1: 1;
	}BAUD;
	struct {
		volatile unsigned int RESERVED1: 2;
		volatile unsigned int RESERVED2: 12;
		volatile unsigned int MA2F: 1;
		volatile unsigned int MA1F: 1;
		volatile unsigned int PF: 1;
		volatile unsigned int FE: 1;
		volatile unsigned int NF: 1;
		volatile unsigned int OR: 1;
		volatile unsigned int IDLE: 1;
		volatile const unsigned int RDRF: 1;
		volatile const unsigned int TC: 1;
		volatile const unsigned int TDRE: 1;
		volatile const unsigned int RAF: 1;
		volatile unsigned int LBKDE: 1;
		volatile unsigned int BRK13: 1;
		volatile unsigned int RWUID: 1;
		volatile unsigned int RXINV: 1;
		volatile unsigned int MSBF: 1;
		volatile unsigned int RXEDGIF: 1;
		volatile unsigned int LBKDIF: 1;
	}STAT;
	struct {
		volatile unsigned int PT: 1;
		volatile unsigned int PE: 1;
		volatile unsigned int ILT: 1;
		volatile unsigned int WAKE: 1;
		volatile unsigned int M: 1;
		volatile unsigned int RSRC: 1;
		volatile unsigned int DOZEEN: 1;
		volatile unsigned int LOOPS: 1;
		volatile unsigned int IDLECFG: 3;
		volatile unsigned int M7: 1;
		volatile unsigned int RESERVED1: 2;
		volatile unsigned int MA2IE: 1;
		volatile unsigned int MA1IE: 1;
		volatile unsigned int SBK : 1;
		volatile unsigned int RWU: 1;
		volatile unsigned int RE: 1;
		volatile unsigned int TE: 1;
		volatile unsigned int ILIE: 1;
		volatile unsigned int RIE: 1;
		volatile unsigned int TCIE: 1;
		volatile unsigned int TIE : 1;
		volatile unsigned int PEIE: 1;
		volatile unsigned int FEIE: 1;
		volatile unsigned int NEIE: 1;
		volatile unsigned int ORIE: 1;
		volatile unsigned int TXINV: 1;
		volatile unsigned int TXDIR: 1;
		volatile unsigned int R9T8: 1;
		volatile unsigned int R8T9: 1;
	}CTRL;
	union{
		volatile unsigned int DATA_REGISTER;
		struct {
			volatile unsigned int R0T0: 1;
			volatile unsigned int R1T1: 1;
			volatile unsigned int R2T2: 1;
			volatile unsigned int R3T3: 1;
			volatile unsigned int R4T4: 1;
			volatile unsigned int R5T5: 1;
			volatile unsigned int R6T6: 1;
			volatile unsigned int R7T7: 1;
			volatile unsigned int R8T8: 1;
			volatile unsigned int R9T9: 1;
			volatile const unsigned int RESERVED1: 1;
			volatile unsigned int IDLINE: 1;
			volatile unsigned int RXEMPT: 1;
			volatile unsigned int FRETSC: 1;
			volatile unsigned int PARITYE: 1;
			volatile unsigned int NOISY: 1;
			volatile const unsigned int RESERVED2: 16;
		}DATA;	
	};

	struct {
		volatile unsigned int MA1: 10;
		volatile const unsigned int RESERVED1: 6;
		volatile unsigned int MA2: 10;
		volatile const unsigned int RESERVED2: 6;
	}MATCH;
	struct {
		volatile unsigned int TXCTSE: 1;
		volatile unsigned int TXRTSE: 1;
		volatile unsigned int TXRTSPOL: 1;
		volatile unsigned int RXRTSE: 1;
		volatile unsigned int TXCTSC: 1;
		volatile unsigned int TXCTSSRC: 1;
		volatile const unsigned int RESERVED1: 2;
		volatile unsigned int RTSWATER: 2;
		volatile const unsigned int RESERVED2: 6;
		volatile unsigned int TNP: 2;
		volatile unsigned int IREN: 1;
		volatile const unsigned int RESERVED3: 13;
	}MODIR;
	struct {
		volatile const unsigned int RXFIFOSIZE: 3;
		volatile unsigned int RXFE: 1;
		volatile const unsigned int TXFIFOSIZE: 3;
		volatile unsigned int TXFE: 1;
		volatile unsigned int RXUFE: 1;
		volatile unsigned int TXOFE: 1;
		volatile unsigned int RXIDEN: 3;
		volatile const unsigned int RESERVED1: 1;
		volatile unsigned int RXFLUSH: 1;
		volatile unsigned int TXFLUSH: 1;
		volatile unsigned int RXUF: 1;
		volatile unsigned int TXOF: 1;
		volatile const unsigned int RESERVED2: 4;
		volatile unsigned int RXEMPT: 1;
		volatile unsigned int TXEMPT: 1;
		volatile const unsigned int RESERVED3: 8;
	}FIFO;
	struct {
		volatile unsigned int TXWATER: 2;
		volatile const unsigned int RESERVED1: 6;
		volatile unsigned int TXCOUNT: 3;
		volatile const unsigned int RESERVED2: 5;
		volatile unsigned int RXWATER: 2;
		volatile const unsigned int RESERVED3: 6;
		volatile unsigned int RXCOUNT: 3;
		volatile const unsigned int RESERVED4: 5;
	}WATER;
}LPUART_t;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/** Base addresses for LPUART instances */
#define LPUART0_BASE        (0x4006A000U)
#define LPUART1_BASE        (0x4006B000U)
#define LPUART2_BASE        (0x4006C000U)

/** Peripheral LPUART base pointers */
#define LPUART0             ((LPUART_t *)(LPUART0_BASE))
#define LPUART1             ((LPUART_t *)(LPUART1_BASE))
#define LPUART2             ((LPUART_t *)(LPUART2_BASE))

#endif /* LPUART_REGISTER_H_ */