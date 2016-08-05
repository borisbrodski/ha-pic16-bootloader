#include <xc.inc>

/* procs reading, erasing and writing flash memory */

psect   text,local,class=CODE,delta=2 ; PIC10/12/16

global _my_flash_read
// SIGNAT _add,4217 ; tell the linker how it should be called (here, use W)
_my_flash_read:
    
    BANKSEL EECON1
    BCF	    CFGS
    BSF	    EEPGD
    BCF	    GIE	    ; (INTCON) disabling interrupts (bootloader doesn't use interrupts)
    BSF	    RD
    
    NOP
    NOP
    
    BSF   GIE	    ; (INTCON) restoring interrupts
   
    
;    BANKSEL EEADRL
;    MOVF    BANKMASK(EEDATL), W
;    BANKSEL _my_flash_data
;    MOVWF   BANKMASK(_my_flash_data), F
;    BANKSEL EEADRL
;    MOVF    BANKMASK(EEDATH), W
;    BANKSEL _my_flash_data
;    MOVWF   BANKMASK(_my_flash_data + 1), F
    
    return
    
    
    
global _my_flash_erase
// SIGNAT _add,4217 ; tell the linker how it should be called (here, use W)
_my_flash_erase:
    BCF	    GIE	    ; (INTCON) disabling interrupts (bootloader doesn't use interrupts)
    
    BANKSEL EECON1
    BSF	    EEPGD   ; (EECON1) Point to program memory
    BCF	    CFGS    ; (EECON1) Not configuration space
    BSF	    FREE    ; (EECON1) Specify erase operation
    BSF	    WREN    ; (EECON1) Enable writes

    MOVLW   55h
    MOVWF   BANKMASK(EECON2)
    MOVLW   0AAh
    MOVWF   BANKMASK(EECON2)
    BSF	    WR	    ; EECON1
    NOP
    NOP
    
    BCF	    WREN    ; Disable writes
    
    BSF   GIE	    ; (INTCON) restoring interrupts
    
    return
    
; Write latch with data to the address EEADR
global _my_flash_write
_my_flash_write:
    BCF	    GIE	    ; (INTCON) disabling interrupts (bootloader doesn't use interrupts)
    
    BANKSEL EECON1
    BSF	    EEPGD   ; (EECON1) Point to program memory
    BCF	    CFGS    ; (EECON1) Not configuration space
    BSF	    WREN    ; (EECON1) Enable writes
    BSF	    LWLO    ; (EECON1) Only Load Write Latches

    MOVLW   55h
    MOVWF   BANKMASK(EECON2)
    MOVLW   0AAh
    MOVWF   BANKMASK(EECON2)
    BSF	    WR	    ; EECON1
    NOP
    NOP
    
    BCF	    WREN    ; Disable writes
    
    BSF   GIE	    ; (INTCON) restoring interrupts
    
    return
    
    