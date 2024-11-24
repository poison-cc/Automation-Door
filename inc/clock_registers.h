#ifndef CLOCK_REGISTER_H
#define CLOCK_REGISTER_H



/**
*   @file    
*   @brief   
*   @details 
*/

/*==================================================================================================
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          
* @details        
*
*/
/**  */
typedef struct {
	volatile const unsigned int SCG_VERID;             /*Offset: 0x00*/
	volatile const unsigned int SCG_PARAM;             /*Offset: 0x04*/
	volatile const unsigned int RESERVED1[2];          /*Offset: 0x08 -> 0x0C*/
	volatile const unsigned int SCG_CSR;               /*Offset: 0x10*/
  union {
    volatile unsigned int SCG_RCCR;           /* Offset: 0x14 */
    struct {
        volatile unsigned int       RCCR_DIVSLOW    : 4;
        volatile unsigned int       RCCR_DIVBUS     : 4;
        volatile const unsigned int RCCR_RESERVED1  : 4;
			  volatile const unsigned int RCCR_RESERVED2  : 4;
			  volatile unsigned int       RCCR_DIVCORE    : 4;
			  volatile const unsigned int RCCR_RESERVED3  : 4;
			  volatile unsigned int       RCCR_SCS        : 4;
			  volatile const unsigned int RCCR_RESERVED4  : 4;
    } RCCR_bits;
  };
  union {
    volatile unsigned int SCG_VCCR;                 /* Offset: 0x18 */
    struct {
        volatile unsigned int       VCCR_DIVSLOW    : 4;
        volatile unsigned int       VCCR_DIVBUS     : 4;
        volatile const unsigned int VCCR_RESERVED1  : 4;
			  volatile const unsigned int VCCR_RESERVED2  : 4;
			  volatile unsigned int       VCCR_DIVCORE    : 4;
			  volatile const unsigned int VCCR_RESERVED3  : 4;
			  volatile unsigned int       VCCR_SCS        : 4;
			  volatile const unsigned int VCCR_RESERVED4  : 4;
    } VCCR_bits;
  };
  union {
    volatile unsigned int SCG_HCCR;                 /* Offset: 0x1c */
    struct {
        volatile unsigned int       HCCR_DIVSLOW    : 4;
        volatile unsigned int       HCCR_DIVBUS     : 4;
        volatile const unsigned int HCCR_RESERVED1  : 4;
			  volatile const unsigned int HCCR_RESERVED2  : 4;
			  volatile unsigned int       HCCR_DIVCORE    : 4;
			  volatile const unsigned int HCCR_RESERVED3  : 4;
			  volatile unsigned int       HCCR_SCS        : 4;
			  volatile const unsigned int HCCR_RESERVED4  : 4;
    } HCCR_bits;
  };
  union {
    volatile unsigned int SCG_CLKOUTCNFG;           /* Offset: 0x20 */
    struct {
        volatile const unsigned int CLKOUTCNFG_RESERVED1  : 24;
        volatile unsigned int       CLKOUTCNFG_CLKOUTSEL  : 4;
			  volatile const unsigned int CLKOUTCNFG_RESERVED2  : 4;
    } SCG_CLKOUTCNFG_bits;
  };
	volatile unsigned const int RESERVED2[55];        /*Offset: 0x24 -> 0xFC*/
  union {
    volatile unsigned int SCG_SOSCCSR;              /* Offset: 0x100 */
    struct {
        volatile unsigned int       SOSCCSR_SOSCEN       : 1 ;
        volatile const unsigned int SOSCCSR_RESERVED1    : 2 ;
			  volatile const unsigned int SOSCCSR_RESERVED2    : 1 ;
			  volatile const unsigned int SOSCCSR_RESERVED3    : 12;
			  volatile unsigned int       SOSCCSR_SOSCCM       : 1 ;
			  volatile unsigned int       SOSCCSR_SOSCCMRE     : 1 ;
			  volatile const unsigned int SOSCCSR_RESERVED4    : 5 ;
			  volatile unsigned int       SOSCCSR_LK           : 1 ;
			  volatile const unsigned int SOSCCSR_SOSCVLD      : 1 ;
			  volatile const unsigned int SOSCCSR_SOSCSEL      : 1 ;
			  volatile unsigned int       SOSCCSR_SOSCCERR     : 1 ;
			  volatile const unsigned int SOSCCSR_RESERVED5    : 5 ;
    } SOSCCSR_bits;
  };
  union {
    volatile unsigned int SCG_SOSCDIV;              /* Offset: 0x104 */
    struct {
        volatile unsigned int       SOSCDIV_SOSCDIV1   : 3 ;
			  volatile const unsigned int SOSCDIV_RESERVED1  : 5 ;
			  volatile unsigned int       SOSCDIV_SOSCDIV2   : 3 ;
        volatile const unsigned int SOSCDIV_RESERVED2  : 5 ;
			  volatile const unsigned int SOSCDIV_RESERVED3  : 3 ;
			  volatile const unsigned int SOSCDIV_RESERVED4  : 13;
    } SOSCDIV_bits;
  };
  union {
    volatile unsigned int SCG_SOSCCFG;              /* Offset: 0x108 */
    struct {
			  volatile const unsigned int SOSCCFG_RESERVED1  : 2 ;
			  volatile unsigned int       SOSCCFG_ERREFS     : 1 ;
        volatile unsigned int       SOSCCFG_HGO        : 1 ;
			  volatile unsigned int       SOSCCFG_RANGE      : 2 ;
			  volatile const unsigned int SOSCCFG_RESERVED2  : 2 ;
			  volatile const unsigned int SOSCCFG_RESERVED3  : 4 ;
			  volatile const unsigned int SOSCCFG_RESERVED4  : 4 ;
			  volatile const unsigned int SOSCCFG_RESERVED5  : 16;
    } SOSCCFG_bits;
  };
	volatile unsigned const int RESERVED3[61];        /*Offset: 0x10C -> 0x1FC*/
  union {
    volatile unsigned int SCG_SIRCCSR;              /* Offset: 0x200 */
    struct {
        volatile unsigned int       SIRCCSR_SOSCEN       : 1 ;
        volatile const unsigned int SIRCCSR_RESERVED1    : 2 ;
			  volatile const unsigned int SIRCCSR_RESERVED2    : 1 ;
			  volatile const unsigned int SIRCCSR_RESERVED3    : 12;
			  volatile unsigned int       SIRCCSR_SOSCCM       : 1 ;
			  volatile unsigned int       SIRCCSR_SOSCCMRE     : 1 ;
			  volatile const unsigned int SIRCCSR_RESERVED4    : 5 ;
			  volatile unsigned int       SIRCCSR_LK           : 1 ;
			  volatile const unsigned int SIRCCSR_SOSCVLD      : 1 ;
			  volatile const unsigned int SIRCCSR_SOSCSEL      : 1 ;
			  volatile unsigned int       SIRCCSR_SOSCCERR     : 1 ;
			  volatile const unsigned int SIRCCSR_RESERVED5    : 5 ;
    } SIRCCSR_bits;
  };
  union {
    volatile unsigned int SCG_SIRCDIV;              /* Offset: 0x204 */
    struct {
        volatile unsigned int       SIRCDIV_SIRCDIV1   : 3 ;
			  volatile const unsigned int SIRCDIV_RESERVED1  : 5 ;
			  volatile unsigned int       SIRCDIV_SIRCDIV2   : 3 ;
        volatile const unsigned int SIRCDIV_RESERVED2  : 5 ;
			  volatile const unsigned int SIRCDIV_RESERVED3  : 3 ;
			  volatile const unsigned int SIRCDIV_RESERVED4  : 13;
    } SIRCDIV_bits;
  };
  union {
    volatile unsigned int SCG_SIRCCFG;              /* Offset: 0x208 */
    struct {
			  volatile const unsigned int SIRCCFG_RESERVED1  : 2 ;
			  volatile unsigned int       SIRCCFG_ERREFS     : 1 ;
        volatile unsigned int       SIRCCFG_HGO        : 1 ;
			  volatile unsigned int       SIRCCFG_RANGE      : 2 ;
			  volatile const unsigned int SIRCCFG_RESERVED2  : 2 ;
			  volatile const unsigned int SIRCCFG_RESERVED3  : 4 ;
			  volatile const unsigned int SIRCCFG_RESERVED4  : 4 ;
			  volatile const unsigned int SIRCCFG_RESERVED5  : 16;
    } SIRCCFG_bits;
  };
	volatile unsigned const int RESERVED4[61];        /*Offset: 0x20C -> 0x2FC*/
  union {
    volatile unsigned int SCG_FIRCCSR;              /* Offset: 0x300 */
    struct {
        volatile unsigned int       FIRCCSR_SOSCEN       : 1 ;
        volatile const unsigned int FIRCCSR_RESERVED1    : 2 ;
			  volatile const unsigned int FIRCCSR_RESERVED2    : 1 ;
			  volatile const unsigned int FIRCCSR_RESERVED3    : 12;
			  volatile unsigned int       FIRCCSR_SOSCCM       : 1 ;
			  volatile unsigned int       FIRCCSR_SOSCCMRE     : 1 ;
			  volatile const unsigned int FIRCCSR_RESERVED4    : 5 ;
			  volatile unsigned int       FIRCCSR_LK           : 1 ;
			  volatile const unsigned int FIRCCSR_SOSCVLD      : 1 ;
			  volatile const unsigned int FIRCCSR_SOSCSEL      : 1 ;
			  volatile unsigned int       FIRCCSR_SOSCCERR     : 1 ;
			  volatile const unsigned int FIRCCSR_RESERVED5    : 5 ;
    } FIRCCSR_bits;
  };
  union {
    volatile unsigned int SCG_FIRCDIV;               /* Offset: 0x304 */
    struct {
        volatile unsigned int       FIRCDIV_FIRCDIV1   : 3 ;
			  volatile const unsigned int FIRCDIV_RESERVED1  : 5 ;
			  volatile unsigned int       FIRCDIV_FIRCDIV2   : 3 ;
        volatile const unsigned int FIRCDIV_RESERVED2  : 5 ;
			  volatile const unsigned int FIRCDIV_RESERVED3  : 3 ;
			  volatile const unsigned int FIRCDIV_RESERVED4  : 13;
    } FIRCDIV_bits;
  };
  union {
    volatile unsigned int SCG_FIRCCFG;               /* Offset: 0x208 */
    struct {
			  volatile const unsigned int FIRCCFG_RESERVED1  : 2 ;
			  volatile unsigned int       FIRCCFG_ERREFS     : 1 ;
        volatile unsigned int       FIRCCFG_HGO        : 1 ;
			  volatile unsigned int       FIRCCFG_RANGE      : 2 ;
			  volatile const unsigned int FIRCCFG_RESERVED2  : 2 ;
			  volatile const unsigned int FIRCCFG_RESERVED3  : 4 ;
			  volatile const unsigned int FIRCCFG_RESERVED4  : 4 ;
			  volatile const unsigned int FIRCCFG_RESERVED5  : 16;
    } FIRCCFG_bits;
  };
	volatile unsigned const int RESERVED5[189];        /*Offset: 0x30C -> 0x5FC*/
  union {
    volatile unsigned int SCG_SPLLCSR;               /* Offset: 0x600 */
    struct {
        volatile unsigned int       SPLLCSR_SPLLEN       : 1 ;
        volatile const unsigned int SPLLCSR_RESERVED1    : 2 ;
			  volatile const unsigned int SPLLCSR_RESERVED2    : 1 ;
			  volatile const unsigned int SPLLCSR_RESERVED3    : 12;
			  volatile unsigned int       SPLLCSR_SPLLCM       : 1 ;
			  volatile unsigned int       SPLLCSR_SPLLCMRE     : 1 ;
			  volatile const unsigned int SPLLCSR_RESERVED4    : 5 ;
			  volatile unsigned int       SPLLCSR_LK           : 1 ;
			  volatile const unsigned int SPLLCSR_SPLLVLD      : 1 ;
			  volatile const unsigned int SPLLCSR_SPLLSEL      : 1 ;
			  volatile unsigned int       SPLLCSR_SPLLCERR     : 1 ;
			  volatile const unsigned int SPLLCSR_RESERVED5    : 5 ;
    } SPLLCSR_bits;
  };
  union {
    volatile unsigned int SCG_SPLLDIV;               /* Offset: 0x304 */
    struct {
        volatile unsigned int       SPLLDIV_SPLLDIV1   : 3 ;
			  volatile const unsigned int SPLLDIV_RESERVED1  : 5 ;
			  volatile unsigned int       SPLLDIV_SPLLDIV2   : 3 ;
        volatile const unsigned int SPLLDIV_RESERVED2  : 5 ;
			  volatile const unsigned int SPLLDIV_RESERVED3  : 3 ;
			  volatile const unsigned int SPLLDIV_RESERVED4  : 13;
    } SPLLDIV_bits;
  };
  union {
    volatile unsigned int SCG_SPLLCFG;                /* Offset: 0x608 */
    struct {
			  volatile unsigned int       SPLLCFG_SOURCE     : 1 ;
			  volatile const unsigned int SPLLCFG_RESERVED1  : 7 ;
			  volatile unsigned int       SPLLCFG_PREDIV     : 3 ;
			  volatile const unsigned int SPLLCFG_RESERVED2  : 5 ;
        volatile unsigned int       SPLLCFG_MULT       : 5 ;
			  volatile const unsigned int SPLLCFG_RESERVED3  : 11 ;

    } SPLLCFG_bits;
  };
} clock_type_t;

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/** Peripheral SCG base address */
#define SCG_BASE             (0x40064000U)


/** Peripheral SCG base pointer */
#define SCG    ((clock_type_t *)SCG_BASE)

#endif