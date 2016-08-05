/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "common.h"

#include <stdio.h>
#include <stdarg.h>


#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "spi.h"
#include "nRF24.h"


#define PROG_START 0x200


asm("PSECT intentry,class=CODE,delta=2 \n"  
    "PAGESEL "___mkstr(PROG_START + 4)" \n"
    "GOTO "___mkstr(PROG_START + 4)" \n");

// TODO Align and put into export-to-app.h
uint8_t leaf_method_var1;


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void my_flash_read();
void my_flash_erase();


#define LED_ON  {}
#define LED_OFF {}



#define COUNT_OF_ELEMENTS_IN_ARRAY(a) (sizeof(a) / sizeof(a[0]))

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    PORTA = 0;
    PORTC = 0;
    LATA = 0;
    LATC = 0;
    ANSELA = 0;
    ANSELC = 0;
    
    spi_init();

    TRISCbits.TRISC4 = 0; // UART

    for (long int i = 0; i < 1000000; i++) {}
    log_string("Starting...\r\n");

    nrf24_init();
    nrf24_set_channel(NRF24_CHANNEL_MIN);
    
    while (true) {
        for (long int i = 0; i < 200000; i++) {}

        log_string("Starting quick com...");
        nrf24_start_quick_com();
        /*
        if (nrf24_login_resp()) {
            for (long int i = 0; i < 1000000; i++) {}
            log_string("Quick com started.");
        }
         */
    }



    
    asm("PAGESEL "___mkstr(PROG_START)" \n"
        "GOTO "___mkstr(PROG_START)" \n");
    
}

