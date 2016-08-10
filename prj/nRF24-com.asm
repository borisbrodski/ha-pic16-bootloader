OPT EXPAND


#include <xc.inc>
#include "nRF24L01.h"
#include "nRF24-com.h"


global _leaf_method_var1 ; Global bank
global _leaf_method_var2 ; Global bank


#define CONFIG_BASIS CONFIG_EN_CRC \
                   | CONFIG_CRCO

#define CONFIG_OFF   CONFIG_BASIS

#define CONFIG_TX    CONFIG_BASIS \
                   | CONFIG_PWR_UP

#define CONFIG_RX    CONFIG_BASIS \
                   | CONFIG_PWR_UP \
                   | CONFIG_PRIM_RX



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


/**
 * Wait for SPI to complete sending.
 * Pre-requirenments:
 * - 
 */
spi_wait MACRO
  LOCAL WAIT1
WAIT1:
  BTFSS  BANKMASK(SSP1STAT), SSP1STAT_BF_POSITION
  GOTO   WAIT1
ENDM

    
    

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
 * Swap n bytes over SPI from buffer.
 * Leave FSR1 pointing to the first byte
 * after the buffer: FSR1 += W.
 *
 * FSR1  - buffer to send
 * W     - buffer size (W>0)
 */
spi_swap:
  BANKSEL(SSP1BUF)
  MOVWF  _leaf_method_var1
  
  spi_slave_on
  
spi_swap_LOOP:
  MOVIW  0[FSR1]
  MOVWF  BANKMASK(SSP1BUF)

  spi_wait

  MOVF   BANKMASK(SSP1BUF), W
  MOVWI  FSR1++
  
  DECFSZ _leaf_method_var1
  GOTO spi_swap_LOOP
  
  spi_slave_off
  return


global _nrf24_com_init
_nrf24_com_init:

    ce_init
    spi_slave_init
    
    MOVLW    LOW init_sequence_start
    MOVWF    FSR1L

    MOVLW    HIGH init_sequence_start
    MOVWF    FSR1H

_nrf24_com_init_LOOP:
    MOVIW    FSR1++
    BTFSC    STATUS, STATUS_Z_POSITION
    RETURN
    CALL     spi_swap

    GOTO     _nrf24_com_init_LOOP



init_sequence_start:
  // 4 bytes addresses
  DB 2
  DB CMD_W_REGISTER | SETUP_AW
  DB SETUP_AW_4BYTES
  
  // Set channel to 103
  DB 2
  DB CMD_W_REGISTER | RF_CH
  DB 103

  DB 2
  DB CMD_W_REGISTER | RF_SETUP
  DB 0 ; 0x23
  
  DB 5
  DB CMD_W_REGISTER | RX_ADDR_P0
  DB (HA_ADDRESS_MY >> 0) & 0xFF
  DB (HA_ADDRESS_MY >> 8) & 0xFF
  DB (HA_ADDRESS_MY >> 16) & 0xFF
  DB (HA_ADDRESS_MY >> 24) & 0xFF

  DB 5
  DB CMD_W_REGISTER | TX_ADDR
  DB (HA_ADDRESS_MASTER >> 0) & 0xFF
  DB (HA_ADDRESS_MASTER >> 8) & 0xFF
  DB (HA_ADDRESS_MASTER >> 16) & 0xFF
  DB (HA_ADDRESS_MASTER >> 24) & 0xFF

  ; End of init sequence
  DB 0






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
