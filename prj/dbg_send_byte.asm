#include <xc.inc>


#ifdef LOG_ACTIVE


; When assembly code is placed in a psect, it can be manipulated as a
; whole by the linker and placed in memory.  
;
; In this example, barfunc is the program section (psect) name, 'local' means
; that the section will not be combined with other sections even if they have
; the same name.  class=CODE means the barfunc must go in the CODE container.
; PIC18's should have a delta (addressible unit size) of 1 (default) since they
; are byte addressible.  PIC10/12/16's have a delta of 2 since they are word
; addressible.  PIC18's should have a reloc (alignment) flag of 2 for any
; psect which contains executable code.  PIC10/12/16's can use the default
; reloc value of 1.  Use one of the psects below for the device you use:


;PSECT bss0, class=BANK0;, abs, ovrld, space=1
;byte1    ds      1

;PSECT data0, class=BANK0;, abs, ovrld, space=1
;byte2    db      3

;  ORG 110h
;global byte
;global i
;i       db      00h

psect   dbgsendfunc,local,class=CODE,delta=2 ; PIC10/12/16

global _dbg_send_byte_b
global _dbg_send_byte_i

byte    equ     _dbg_send_byte_b
i       equ     _dbg_send_byte_i


#define PAUSE1 1
#define PAUSE2 1
#define PAUSE3 2
#define PAUSE4 5

global _dbg_byte_to_write


global _dbg_send_byte
_dbg_send_byte:

        BANKSEL(_dbg_byte_to_write)
        movf    BANKMASK(_dbg_byte_to_write),w

        BANKSEL(byte)
        movwf   BANKMASK(byte),f
        movlw   8
        movwf   BANKMASK(i),f

        ; Disable interrupts
        BANKSEL(INTCON)
        BCF     GIE

        BANKSEL(PORTC)
        BCF     PORTC, 4

        NOP
        movlw   PAUSE1
P1:     addlw   -1
        BTFSS   ZERO
        GOTO    P1

        BANKSEL(byte)
I_LOOP:
        movlw   PAUSE2
P2:     addlw   -1
        BTFSS   ZERO
        GOTO    P2

        BTFSS   BANKMASK(byte),0
        GOTO    SEND_1
        GOTO    SEND_0

SEND_0:
        NOP
        NOP
        BANKSEL(PORTC)	; select bank0
        BSF     PORTC, 4

        ; Shift
        BANKSEL(byte)
        LSRF BANKMASK(byte)

        DECFSZ  BANKMASK(i),f
        GOTO    I_LOOP
        GOTO    FINISH

SEND_1:
        NOP
        NOP
        NOP

        ; Set
        BANKSEL(PORTC)	; select bank0
        BCF     PORTC, 4

        ; Shift
        BANKSEL(byte)
        LSRF BANKMASK(byte)

        DECFSZ  BANKMASK(i),f
        GOTO    I_LOOP
        GOTO    FINISH
        NOP
FINISH:

        movlw   PAUSE3
P3:     addlw   -1
        BTFSS   ZERO
        GOTO    P3

        NOP

        BANKSEL(PORTC)	; select bank0
        BSF     PORTC, 4

        ; Enable interrupts
        BANKSEL(INTCON)
        BSF     GIE

        movlw   PAUSE4
P4:     addlw   -1
        BTFSS   ZERO
        GOTO    P4

        return


#endif