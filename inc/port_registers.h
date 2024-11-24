/**
*   @file    Port_Registers.h
*   @brief   Definition of Registers for the PORT module
*   @details This file provides the structure definition for the PORT module registers,
*            base addresses for PORTA to PORTE, and macro definitions for accessing the registers.
*            It also includes guards to prevent multiple declarations.
*/

/*==================================================================================================
*                                        INCLUDE GUARDS
==================================================================================================*/

#ifndef PORT_REGISTER_H
#define PORT_REGISTER_H

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          Structure name: Port_Type
* @details        This structure defines the layout of the PORT registers, 
*                 which includes Pin Control Registers (PCR), Global Pin Control Low (GPCLR),
*                 Global Pin Control High (GPCHR), and Interrupt Status Flag Register (ISFR).
*
*/
/** PORT - Register Layout Typedef */
typedef struct {
    union {
        // Pin Control Register (PCR) for individual pins
        volatile unsigned int PCR0_Reg;    /**< Full Pin Control Register 0 */
        
        struct {
            volatile unsigned int PS : 1;           /**< Pull Select: 0 = Internal pull-down, 1 = Internal pull-up */
            volatile unsigned int PE : 1;           /**< Pull Enable: 0 = No pull resistor, 1 = Enable pull resistor */
            volatile unsigned int RESERVED1 : 1;    /**< Reserved bit */
            volatile const unsigned int RESERVED2 : 1; /**< Reserved constant bit */
            volatile unsigned int PFE : 1;          /**< Passive Filter Enable: 0 = Disabled, 1 = Enabled */
            volatile unsigned int RESERVED3 : 1;    /**< Reserved bit */
            volatile unsigned int DSE : 1;          /**< Drive Strength Enable: 0 = Low drive strength, 1 = High drive strength */
            volatile const unsigned int RESERVED4 : 1; /**< Reserved constant bit */
            volatile unsigned int MUX : 3;          /**< Pin Mux Control: Selects the function of the pin (e.g., GPIO, alternative functions) */
            volatile const unsigned int RESERVED5 : 4; /**< Reserved constant bits */
            volatile unsigned int LK : 1;           /**< Lock Register: 0 = Pin Control Register is not locked, 1 = Pin Control Register is locked */
            volatile unsigned int IQRC : 4;         /**< Interrupt Configuration: Selects the interrupt configuration for the pin */
            volatile const unsigned int RESERVED6 : 4; /**< Reserved constant bits */
            volatile unsigned int ISF : 1;          /**< Interrupt Status Flag: 0 = No interrupt, 1 = Interrupt detected */
            volatile const unsigned int RESERVED7 : 7; /**< Reserved constant bits */
        } PORT_PCR_Fields;                          /**< Detailed bit fields for PCR */
    } PCR[32];                                      /**< Array of 32 Pin Control Registers (PCR) for each pin */
    
    union {
        // Global Pin Control Low Register (GPCLR)
        volatile unsigned int GPCLR_Reg;            /**< Full Global Pin Control Low Register */
        
        struct {
            volatile unsigned int GPWD : 16;        /**< Global Pin Write Data: Data to be written to all configured pins */
            volatile unsigned int GPWE : 16;        /**< Global Pin Write Enable: Enables write to corresponding bits in PCR registers */
        } GPCLR;                                    /**< Detailed bit fields for GPCLR */
    };
    
    union {
        // Global Pin Control High Register (GPCHR)
        volatile unsigned int GPCHR_Reg;            /**< Full Global Pin Control High Register */
        
        struct {
            volatile unsigned int GPWD : 16;        /**< Global Pin Write Data: Data to be written to all configured pins */
            volatile unsigned int GPWE : 16;        /**< Global Pin Write Enable: Enables write to corresponding bits in PCR registers */
        } GPCHR;                                    /**< Detailed bit fields for GPCHR */
    };
    
    union {
        // Global Interrupt Control Low Register (GICLR)
        volatile unsigned int GICLR_Reg;            /**< Full Global Interrupt Control Low Register */
        
        struct {
            volatile unsigned int GPWE : 16;        /**< Global Pin Write Enable: Enables interrupt write to corresponding bits */
            volatile unsigned int GPWD : 16;        /**< Global Pin Write Data: Data to be written for interrupts */
        } GICLR;                                    /**< Detailed bit fields for GICLR */
    };
    
    union {
        // Global Interrupt Control High Register (GICHR)
        volatile unsigned int GICHR_Reg;            /**< Full Global Interrupt Control High Register */
        
        struct {
            volatile unsigned int GPWE : 16;        /**< Global Pin Write Enable: Enables interrupt write to corresponding bits */
            volatile unsigned int GPWD : 16;        /**< Global Pin Write Data: Data to be written for interrupts */
        } GICHR;                                    /**< Detailed bit fields for GICHR */
    };
    
    volatile const unsigned int RESERVED[4];        /**< Reserved space for memory alignment */
    volatile unsigned int ISFR;                     /**< Interrupt Status Flag Register: Reflects the status of interrupts for all pins */
    volatile const unsigned int RESERVED1[7];       /**< Additional reserved space */
    volatile unsigned int DFER;                     /**< Digital Filter Enable Register: Enables/disables the digital filter for each pin */
    volatile unsigned int DFCR;                     /**< Digital Filter Clock Register: Configures the clock source for the digital filter */
    volatile unsigned int DFWR;                     /**< Digital Filter Width Register: Configures the width of the digital filter */
} Port_Type;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/** @brief Base addresses for PORT peripherals */
#define PORTA_BASE                               (0x40049000U)   /**< PORTA base address */
#define PORTB_BASE                               (0x4004A000U)   /**< PORTB base address */
#define PORTC_BASE                               (0x4004B000U)   /**< PORTC base address */
#define PORTD_BASE                               (0x4004C000U)   /**< PORTD base address */
#define PORTE_BASE                               (0x4004D000U)   /**< PORTE base address */

/** @brief Pointers to PORT peripherals */
#define PORTA    ((Port_Type *)PORTA_BASE)       /**< Pointer to PORTA base address */
#define PORTB    ((Port_Type *)PORTB_BASE)       /**< Pointer to PORTB base address */
#define PORTC    ((Port_Type *)PORTC_BASE)       /**< Pointer to PORTC base address */
#define PORTD    ((Port_Type *)PORTD_BASE)       /**< Pointer to PORTD base address */
#define PORTE    ((Port_Type *)PORTE_BASE)       /**< Pointer to PORTE base address */

#endif /* PORT_REGISTER_H */