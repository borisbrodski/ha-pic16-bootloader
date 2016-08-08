OPT EXPAND


#include <xc.inc>
#include "nRF24L01.h"
#include "nRF24-com.h"


global _leaf_method_var1 ; Global bank
global _leaf_method_var2 ; Global bank


psect   barfunc,local,class=CODE,delta=2 ; PIC10/12/16
// psect   barfunc,local,class=CODE,reloc=2 ; PIC18


// Chip enable PIN
ce_on MACRO
    BANKSEL(CE_PORT)
    BSF     BANKMASK(CE_PORT), CE_PIN
ENDM


ce_off MACRO
    BANKSEL(CE_PORT)
    BCF     BANKMASK(CE_PORT), CE_PIN
ENDM


ce_init MACRO
    BANKSEL(CE_TRIS)
    BCF     BANKMASK(CE_TRIS), CE_PIN
ENDM


// Inverted SPI slave select PIN
spi_slave_on MACRO
    BANKSEL(SPI_SLAVE_PORT)
    BCF     BANKMASK(SPI_SLAVE_PORT), SPI_SLAVE_PIN
ENDM


spi_slave_off MACRO
    BANKSEL(SPI_SLAVE_PORT)
    BSF     BANKMASK(SPI_SLAVE_PORT), SPI_SLAVE_PIN
ENDM


spi_slave_init MACRO
    spi_slave_off
    BANKSEL(SPI_SLAVE_TRIS)
    BCF     BANKMASK(SPI_SLAVE_TRIS), SPI_SLAVE_PIN
ENDM


spi_wait MACRO
  LOCAL WAIT1
WAIT1:
  BTFSS  BANKMASK(SSP1STAT), SSP1STAT_BF_POSITION
  GOTO   WAIT1
ENDM
     
    
global _nrf24_com_init
_nrf24_com_init:

    ce_init
    spi_slave_init
    
    wreg_c     RF_CH, 103
    
    
    return

    
    

/**
 * Write 1 byte register.
 *
 * W                  - RF24 register
 * _leaf_method_var1  - Value to write
 */
wreg8:
  IORLW  CMD_W_REGISTER
    
  spi_slave_on

  BANKSEL(SSP1BUF)
  MOVWF  BANKMASK(SSP1BUF)

  spi_wait

  MOVF   BANKMASK(_leaf_method_var1), W
  MOVWF  BANKMASK(SSP1BUF)

  spi_wait

  spi_slave_off
  return


/**
 * Read 1 byte register.
 *
 * W            - RF24 register
 * Out: SSP1BUF - Value
 */
rreg8:
  ; IORLW  CMD_R_REGISTER
  BANKSEL(SSP1BUF)
  MOVWF  BANKMASK(SSP1BUF)

  spi_wait
  
  MOVWF  BANKMASK(SSP1BUF)

  spi_wait
  return


/**
 * Write own 4 byte address
 * W - register
 */











// When assembly code is placed in a psect, it can be manipulated as a
// whole by the linker and placed in memory.  
//
// In this example, barfunc is the program section (psect) name, 'local' means
// that the section will not be combined with other sections even if they have
// the same name.  class=CODE means the barfunc must go in the CODE container.
// PIC18's should have a delta (addressible unit size) of 1 (default) since they
// are byte addressible.  PIC10/12/16's have a delta of 2 since they are word
// addressible.  PIC18's should have a reloc (alignment) flag of 2 for any
// psect which contains executable code.  PIC10/12/16's can use the default
// reloc value of 1.  Use one of the psects below for the device you use:
