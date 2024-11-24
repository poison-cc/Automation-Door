/**
*   @file    I2C_Registers.h
*   @brief   Definition of Registers for the I2C Module
*   @details This header file contains the structure definitions for the I2C peripheral 
*            registers. It includes the base addresses for I2C peripherals and provides 
*            type definitions to access individual I2C registers and their respective 
*            bit fields.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef I2C_REGISTER_H_
#define I2C_REGISTER_H_

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          I2C_Type
* @details        This structure represents the I2C peripheral registers and their offsets. 
*                 It provides a convenient way to access and manipulate I2C registers using 
*                 bit fields.
*/
/* I2C - Register Layout Typedef */

typedef struct {
	union {
		volatile const unsigned int VERID_REGISTER;
		struct {
			volatile const unsigned int FEATURE: 16;
			volatile const unsigned int MINOR: 8;
			volatile const unsigned int MAJOR: 8;
		}VERID;
	};
	union {
		volatile const unsigned int PARAM_REGISTER;
		struct {
			volatile const unsigned int MTXFIFO: 4;
			volatile const unsigned int RESERVED1: 4;
			volatile const unsigned int MRXFIFO: 4;
			volatile const unsigned int RESERVED2: 4;
			volatile const unsigned int RESERVED3: 16;
		}PARAM;
	};
	volatile unsigned int RESERVED1[2];
	union {
		volatile unsigned int MCR_REGISTER;
		struct{
			volatile unsigned int MEN: 1;
			volatile unsigned int RST: 1;
			volatile unsigned int DOZEN: 1;
			volatile unsigned int DBGEN: 1;
			volatile const unsigned int RESERVED1: 4;
			volatile unsigned int RTF: 1;
			volatile unsigned int RRF:1;
			volatile const unsigned int RESERVED2 : 22;
		}MCR;
	};
	union {
		volatile unsigned int MSR_REGISTER;
		struct{
			volatile const unsigned int TDF: 1;
			volatile const unsigned int RDF: 1;
			volatile const unsigned int RESERVED1: 6;
			volatile unsigned int EPF: 1;
			volatile unsigned int SDF: 1;
			volatile unsigned int NDF: 1;
			volatile unsigned int ALE: 1;
			volatile unsigned int FEF:1;
			volatile unsigned int PLTF : 1;
			volatile unsigned int DMF : 1;
			volatile const unsigned int RESERVED2: 9;
			volatile const unsigned int MBF: 1;
			volatile const unsigned int BBF: 1;
			volatile const unsigned int RESERVED3: 6;
		}MSR;
	};
	union {
		volatile unsigned int MIER_REGISTER;
		struct{
			volatile unsigned int TDIE: 1;
			volatile unsigned int RDIE: 1;
			volatile const unsigned int RESERVED1: 6;
			volatile unsigned int EPIE: 1;
			volatile unsigned int SDIE: 1;
			volatile unsigned int NDIE: 1;
			volatile unsigned int ALIE: 1;
			volatile unsigned int FEIE:1;
			volatile unsigned int PLTIE : 1;
			volatile unsigned int DMIE : 1;
			volatile const unsigned int RESERVED2: 17;
		}MIER;
	};
	union {
		volatile unsigned int MDER_REGISTER;
		struct{
			volatile unsigned int TDDR: 1;
			volatile unsigned int RDDE: 1;
			volatile const unsigned int RESERVED: 30;
		}MDER;
	};
	union {
		volatile unsigned int MCFGR0_REGISTER;
		struct{
			volatile unsigned int HREN: 1;
			volatile unsigned int HRPOL: 1;
			volatile unsigned int HRSEL: 1;
			volatile const unsigned int RESERVED1: 5;
			volatile unsigned int CIRFIFO: 1;
			volatile unsigned int RDMO: 1;
			volatile const unsigned int RESERVED2: 22;
		}MCFGR0;
	};
	union {
		volatile unsigned int MCFGR1_REGISTER;
		struct{
			volatile unsigned int PRESCALER: 3;
			volatile const unsigned int RESERVED1: 5;
			volatile unsigned int AUTOSTOP: 1;
			volatile unsigned int IGNACK: 1;
			volatile unsigned int TIMECFG: 1;
			volatile const unsigned int RESERVED2: 5;
			volatile unsigned int MATCFG: 3;
			volatile const unsigned int RESERVED3: 5;
			volatile unsigned int PINCFG: 3;
			volatile const unsigned int RESERVED4: 1;
			volatile const unsigned int RESERVED5: 4;
		}MCFGR1;
	};
	union {
		volatile unsigned int MCFGR2_REGISTER;
		struct{
			volatile unsigned int BUSIDLE: 12;
			volatile const unsigned int RESERVED1: 4;
			volatile unsigned int FILTSCL: 4;
			volatile const unsigned int RESERVED2: 4;
			volatile unsigned int FILTSDA: 4;
			volatile const unsigned int RESERVED3: 4;
		}MCFGR2;
	};
	union {
		volatile unsigned int MCFGR3_REGISTER;
		struct{
			volatile const unsigned int RESERVED1: 8;
			volatile unsigned int PINLOW: 12;
			volatile const unsigned int RESERVED2: 12;
		}MCFGR3;
	};
	volatile unsigned int RESERVED2[4];
	union {
		volatile unsigned int MDMR_REGISTER;
		struct{
			volatile unsigned int MATCH0: 8;
			volatile const unsigned int RESERVED1: 8;
			volatile unsigned int MATCH1: 8;
			volatile const unsigned int RESERVED2: 8;
		}MDMR;
	};
	volatile unsigned int RESERVED3;
	union {
		volatile unsigned int MCCR0_REGISTER;
		struct{
			volatile unsigned int CLKLO: 6;
			volatile const unsigned int RESERVED1: 2;
			volatile unsigned int CLKHI: 6;
			volatile const unsigned int RESERVED2: 2;
			volatile unsigned int SETHOLD: 6;
			volatile const unsigned int RESERVED3: 2;
			volatile unsigned int DATAVD: 6;
			volatile const unsigned int RESERVED4: 2;
		}MCCR0;
	};
	volatile unsigned int RESERVED4;
	union {
		volatile unsigned int MCCR1_REGISTER;
		struct{
			volatile unsigned int CLKLO: 6;
			volatile const unsigned int RESERVED1: 2;
			volatile unsigned int CLKHI: 6;
			volatile const unsigned int RESERVED2: 2;
			volatile unsigned int SETHOLD: 6;
			volatile const unsigned int RESERVED3: 2;
			volatile unsigned int DATAVD: 6;
			volatile const unsigned int RESERVED4: 2;
		}MCCR1;
	};
	volatile unsigned int RESERVED5;
	union {
		volatile unsigned int MFCR_REGISTER;
		struct{
			volatile unsigned int TXWATER: 2;
			volatile const unsigned int RESERVED1: 14;
			volatile unsigned int RXWATER: 2;
			volatile const unsigned int RESERVED2: 14;
		}MFCR;
	};
	union {
		volatile const unsigned int MFSR_REGISTER;
		struct{
			volatile const unsigned int TXCOUNT: 3;
			volatile const unsigned int RESERVED1: 13;
			volatile const unsigned int RXCOUNT: 3;
			volatile const unsigned int RESERVED2: 13;
		}MFSR;
	};
	union {
		volatile unsigned int MTDR_REGISTER;
		struct{
			volatile unsigned int DATA: 8;
			volatile unsigned int CMD: 8;
			volatile const unsigned int RESERVED1: 16;
		}MTDR;
	};
	volatile unsigned int RESERVED6[3];
	union {
		volatile const unsigned int MRDR_REGISTER;
		struct{
			volatile const unsigned int DATA: 8;
			volatile const unsigned int RESERVED1: 6;
			volatile const unsigned int RXEMPTY: 1;
			volatile const unsigned int RESERVED2: 17;
		}MRDR;
	};
	volatile unsigned int RESERVED7[39];
	union {
		volatile unsigned int SCR_REGISTER;
		struct{
			volatile unsigned int SEN: 1;
			volatile unsigned int RST: 1;
			volatile const unsigned int RESERVED1: 2;
			volatile unsigned int FILTEN: 1;
			volatile unsigned int FILTDZ: 1;
			volatile const unsigned int RESERVED2: 2;
			volatile unsigned int RTF: 1;
			volatile unsigned int RRF: 1;
			volatile const unsigned int RESERVED3: 22;
		}SCR;
	};
	union {
		volatile unsigned int SSR_REGISTER;
		struct{
			volatile const unsigned int TDF: 1;
			volatile const unsigned int RDF: 1;
			volatile const unsigned int AVF: 1;
			volatile const unsigned int TAF: 1;
			volatile const unsigned int RESERVED1: 4;
			volatile unsigned int RSF: 1;
			volatile unsigned int SDF: 1;
			volatile unsigned int BEF: 1;
			volatile unsigned int FEF: 1;
			volatile const unsigned int AM0F: 1;
			volatile const unsigned int AM1F: 1;
			volatile const unsigned int GCF: 1;
			volatile const unsigned int SARF: 1;
			volatile const unsigned int RESERVED2: 8;
			volatile const unsigned int SBF: 1;
			volatile const unsigned int BBF: 1;
			volatile const unsigned int RESERVED3: 6;
		}SSR;
	};
	union {
		volatile unsigned int SIER_REGISTER;
		struct{
			volatile unsigned int TDIE: 1;
			volatile unsigned int RDIE: 1;
			volatile unsigned int AVIE: 1;
			volatile unsigned int TAIE: 1;
			volatile const unsigned int RESERVED1: 4;
			volatile unsigned int FILTEN: 1;
			volatile unsigned int FILTDZ: 1;
			volatile unsigned int RSIE: 1;
			volatile unsigned int SDIE: 1;
			volatile unsigned int BEIE: 1;
			volatile unsigned int FEIE: 1;
			volatile unsigned int AM0IE: 1;
			volatile unsigned int AM1F: 1;
			volatile unsigned int GCIE: 1;
			volatile unsigned int SARIE: 1;
			volatile const unsigned int RESERVED2: 16;
		}SIER;
	};
	union {
		volatile unsigned int SDER_REGISTER;
		struct{
			volatile unsigned int TDDE: 1;
			volatile unsigned int RDDE: 1;
			volatile unsigned int AVDE: 1;
			volatile const unsigned int RESERVED1: 29;
		}SDER;
	};
	volatile unsigned int RESERVED8;
	union {
		volatile unsigned int SCFGR1_REGISTER;
		struct{
			volatile unsigned int ADRSTALL: 1;
			volatile unsigned int RXSTALL: 1;
			volatile unsigned int TXDSTALL: 1;
			volatile unsigned int ACKSTALL: 1;
			volatile const unsigned int RESERVED1: 4;
			volatile unsigned int GCEN: 1;
			volatile unsigned int SAEN: 1;
			volatile unsigned int TXCFG: 1;
			volatile unsigned int RXCFG: 1;
			volatile unsigned int IGNACK: 1;
			volatile unsigned int HSMEN: 1;
			volatile const unsigned int RESERVED2: 2;
			volatile unsigned int ADDRCFG: 3;
			volatile const unsigned int RESERVED3: 13;
		}SCFGR1;
	};
	union {
		volatile unsigned int SCFGR2_REGISTER;
		struct{
			volatile unsigned int CLKHOLD: 4;
			volatile const unsigned int RESERVED1: 4;
			volatile unsigned int DATAVD: 6;
			volatile const unsigned int RESERVED2: 2;
			volatile unsigned int FILTSCL: 4;
			volatile const unsigned int RESERVED3: 4;
			volatile unsigned int FILTSDA: 4;
			volatile const unsigned int RESERVED4: 4;
		}SCFGR2;
	};
	volatile unsigned int RESERVED9[5];
	union {
		volatile unsigned int SAMR_REGISTER;
		struct{
			volatile const unsigned int RESERVED1: 1;
			volatile unsigned int ADDR0: 10;
			volatile const unsigned int RESERVED2: 5;
			volatile const unsigned int RESERVED3: 1;
			volatile unsigned int ADDR1: 10;
			volatile const unsigned int RESERVED4: 5;
		}SAMR;
	};
	volatile unsigned int RESERVED10[3];
	union {
		volatile const unsigned int SASR_REGISTER;
		struct{
			volatile const unsigned int RADDR: 11;
			volatile const unsigned int RESERVED1: 3;
			volatile const unsigned int ANV: 1;
			volatile const unsigned int RESERVED2: 17;
		}SASR;
	};
	union {
		volatile const unsigned int STAR_REGISTER;
		struct{
			volatile unsigned int TXNACK: 1;
			volatile const unsigned int RESERVED1: 31;
		}STAR;
	};
	volatile unsigned int RESERVED11[2];
	union {
		volatile unsigned int STDR_REGISTER;
		struct{
			volatile unsigned int DATA: 8;
			volatile unsigned int RESERVED1: 24;
		}STDR;
	};
	volatile unsigned int RESERVED12[3];
	union {
		volatile const unsigned int SRDR_REGISTER;
		struct{
			volatile const unsigned int DATA: 8;
			volatile const unsigned int RESERVED1: 6;
			volatile const unsigned int RXEMPTY: 1;
			volatile const unsigned int SOF: 1;
			volatile const unsigned int RESERVED2: 16;
		}SRDR;
	};
}I2C_Type;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
* @brief Base addresses for I2C peripherals
*/
#define LPI2C0_BASE   (0x40066000U)  /**< Base address for LPI2C0 */
#define LPI2C1_BASE   (0x40067000U)  /**< Base address for LPI2C1 */

/**
* @brief Peripheral I2C base pointers
*/
#define LPI2C0        ((I2C_Type*)(LPI2C0_BASE))  /**< Peripheral base pointer for LPI2C0 */
#define LPI2C1        ((I2C_Type*)(LPI2C1_BASE))  /**< Peripheral base pointer for LPI2C1 */

#endif /* I2C_REGISTER_H_ */