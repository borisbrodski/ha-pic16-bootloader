#define SPI_C
#include "common.h"
#include "spi.h"

void spi_init() {
    TRISCbits.TRISC0 = 0; // SPI-CLOCK
    TRISCbits.TRISC1 = 1; // Master In Slave Out
    TRISCbits.TRISC2 = 0; // Master Out Slave In
    
    SPI_SLAVE_INIT

    APFCON0bits.SDOSEL = 0; // Select RC2 for DSO (Master Out Slave In)

    SSP1CON1bits.CKP = 0; // Idle state for clock is a low level
    SSP1STATbits.CKE = 1; // Clock - change pins on high->low
    SSP1STATbits.SMP = 0 ; // 1 - Input data sampled at the end of data output time, 0 - middle
    SSP1CON1bits.SSPM = 0b00000010; // Master mode, Fosc/64
    SSP1CON1bits.SSPEN = 1;
}

void spi_swap() {
    log_printf(">>>>: %b\r\n", spi.buffer, spi_buffer_size);

    SPI_SLAVE_ON
    for (unsigned char i = 0; i < spi_buffer_size; i++) {
        SSP1BUF = spi.buffer[i];
        while (!SSP1STATbits.BF) {}
        spi.buffer[i] = SSP1BUF;
    }
    SPI_SLAVE_OFF

    log_printf("<<<<: %b\r\n", spi.buffer, spi_buffer_size);
}

void spi_send() {
    log_printf(">>>>: %b\r\n", spi.buffer, spi_buffer_size);

    SPI_SLAVE_ON
    for (unsigned char i = 0; i < spi_buffer_size; i++) {
        SSP1BUF = spi.buffer[i];
        while (!SSP1STATbits.BF) {}
    }
    SPI_SLAVE_OFF
}

//uint8_t spi_swap_byte(uint8_t b) {
//    spi_buffer[0] = b;
//    spi_buffer_size = 1;
//    spi_swap();
//    return spi_buffer[0];
//}
