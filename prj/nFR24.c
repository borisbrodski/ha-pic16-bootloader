#include "common.h"
#include "nRF24L01.h"
#include "nRF24.h"

#include <string.h>

#include "spi.h"

#define CONFIG_BASIS    CONFIG_EN_CRC       /* Enable CRC  */ \
                      | CONFIG_CRCO         /* 2 bytes CRC */

#define POWER_OFF       { nrf24_wreg(CONFIG, CONFIG_BASIS);}
#define POWER_SEND      { nrf24_wreg(CONFIG, CONFIG_BASIS | CONFIG_PWR_UP); }
#define POWER_RECIEVE   { nrf24_wreg(CONFIG, CONFIG_BASIS | CONFIG_PWR_UP | CONFIG_PRIM_RX); }

#define MIN(a, b)       (a > b ? b : a)
#define SEND_SSP1BUF    {while (!SSP1STATbits.BF) {}}

/*
103
STATUS		 = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
RX_ADDR_P0-1	 = 0x65646f4e31 0x65646f4e32
RX_ADDR_P2-5	 = 0xc3 0xc4 0xc5 0xc6
TX_ADDR		 = 0x65646f4e31
RX_PW_P0-6	 = 0x20 0x20 0x00 0x00 0x00 0x00
EN_AA		 = 0x3f
EN_RXADDR	 = 0x02
RF_CH		 = 0x67
RF_SETUP	 = 0x23
CONFIG		 = 0x0f
DYNPD/FEATURE	 = 0x3f 0x06
Data Rate	 = 250KBPS
Model		 = nRF24L01+
CRC Length	 = 16 bits
PA Power	 = PA_LOW
 
 */


static inline void nrf24_wreg(uint8_t reg, uint8_t val) {
    spi_buffer_size = 2;
    spi.buffer[0] = CMD_W_REGISTER | (REGISTER_MASK & reg);
    spi.buffer[1] = val;
    spi_swap();
}

static void nrf24_rreg(uint8_t reg) {
    SPI_SLAVE_ON
    SSP1BUF = CMD_R_REGISTER | (REGISTER_MASK & reg);
    SEND_SSP1BUF
    SSP1BUF = 0;
    SEND_SSP1BUF
    SPI_SLAVE_OFF
}

static void nrf24_print_address(uint8_t reg) {
    SPI_SLAVE_ON
    SSP1BUF = CMD_R_REGISTER | (REGISTER_MASK & reg);
    uint8_t buf[4];
    SEND_SSP1BUF
    for (uint8_t i = 0; i < 4; i++) {
        SSP1BUF = 0;
        SEND_SSP1BUF
        buf[i] = SSP1BUF;
    }
    log_printf(" 0x%x%x%x%x", buf[3], buf[2], buf[1], buf[0]);
//    SSP1BUF = 0;
//    while (!SSP1STATbits.BF) {}
//    log_printf("%x", SSP1BUF);
//    SSP1BUF = 0;
//    while (!SSP1STATbits.BF) {}
//    log_printf("%x", SSP1BUF);
//    SSP1BUF = 0;
//    while (!SSP1STATbits.BF) {}
//    log_printf("%x", SSP1BUF);
    SPI_SLAVE_OFF
}

/*
STATUS		 = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
RX_ADDR_P0-1	 = 0x646f4e31 0x2fd0b555
RX_ADDR_P2-5	 = 0xc3 0xc4 0xc5 0xc6
TX_ADDR		 = 0x646f4e31
RX_PW_P0-6	 = 0x20 0x20 0x00 0x00 0x00 0x00
EN_AA		 = 0x00
EN_RXADDR	 = 0x02
RF_CH		 = 0x67
RF_SETUP	 = 0x23
CONFIG		 = 0x0f
DYNPD/FEATURE	 = 0x00 0x00
Data Rate	 = 250KBPS
Model		 = nRF24L01+
CRC Length	 = 16 bits
PA Power	 = PA_LOW
 
 */
void nrf24_print() {
    /*
    nrf24_rreg(STATUS);
    log_printf("STATUS  \t = 0x%x\r\n", SSP1BUF);
    log_printf("RX_ADDR_P0-1\t =");
    nrf24_print_address(RX_ADDR_P0);
    nrf24_print_address(RX_ADDR_P1);
    nrf24_rreg(RX_ADDR_P2);
    log_printf("\r\nRX_ADDR_P2-5	 = 0x%x", SSP1BUF);
    nrf24_rreg(RX_ADDR_P3);
    log_printf(" 0x%x", SSP1BUF);
    nrf24_rreg(RX_ADDR_P4);
    log_printf(" 0x%x", SSP1BUF);
    nrf24_rreg(RX_ADDR_P5);
    log_printf(" 0x%x\r\n", SSP1BUF);
    log_printf("TX_ADDR		 =");
    nrf24_print_address(TX_ADDR);
    
    nrf24_rreg(RX_PW_P0);
    log_printf("\r\nRX_PW_P0-6	 = 0x%x", SSP1BUF);
    nrf24_rreg(RX_PW_P1);
    log_printf(" 0x%x", SSP1BUF);
    nrf24_rreg(RX_PW_P2);
    log_printf(" 0x%x", SSP1BUF);
    nrf24_rreg(RX_PW_P3);
    log_printf(" 0x%x", SSP1BUF);
    nrf24_rreg(RX_PW_P4);
    log_printf(" 0x%x", SSP1BUF);
    nrf24_rreg(RX_PW_P5);
    log_printf(" 0x%x\r\n", SSP1BUF);
    
    nrf24_rreg(EN_AA);
    log_printf("EN_AA		 = 0x%x\r\n", SSP1BUF);
    
    nrf24_rreg(EN_RXADDR);
    log_printf("EN_RXADDR	 = 0x%x\r\n", SSP1BUF);
    
    nrf24_rreg(RF_CH);
    log_printf("RF_CH		 = 0x%x\r\n", SSP1BUF);
    
    nrf24_rreg(RF_SETUP);
    log_printf("RF_SETUP	 = 0x%x\r\n", SSP1BUF);
    
    nrf24_rreg(CONFIG);
    log_printf("CONFIG		 = 0x%x\r\n", SSP1BUF);
    
    nrf24_rreg(DYNPD);
    log_printf("DYNPD/FEATURE	 = 0x%x", SSP1BUF);
    nrf24_rreg(FEATURE);
    log_printf(" 0x%x\r\f", SSP1BUF);
     */
}

void nrf24_configure_for_start_com() {
    nrf24_wreg(EN_AA, 0); // Disable Auto ack
}

void nrf24_configure_quick_com() {
    nrf24_wreg(EN_AA, 1); // Enable Auto ack
    nrf24_wreg(SETUP_RETR, SETUP_RETR_ARC_15 | SETUP_RETR_ARD_1500);    // Retry 15 times, wait 1.5ms
}

//void nrf24_init() {
//    CE_INIT
//    
//}
void nrf24_init() {

    // Init timer4
    
//    T4CONbits.T4OUTPS = 0b1111; // 1:16
//    T4CONbits.T4CKPS = 0b11; // 1:64
//    T4CONbits.TMR4ON = 1; //
    T4CON = 0b01111111;
            
    // Init nRF24L01+
    
    CE_INIT
    
    POWER_OFF;


    
    for (long int i = 0; i < 10000; i++) {}
    nrf24_wreg(SETUP_AW, SETUP_AW_4BYTES);  // 4 bytes addresses
    
    //nrf24_wreg(RF_SETUP, RF_SETUP_RF_DR_250 | RF_SETUP_RF_PWR_0);
    //nrf24_wreg(RF_SETUP, 0x23);
    nrf24_wreg(RF_SETUP, 0b00000000);
    
    //nrf24_wreg(EN_RXADDR, EN_RXADDR_ERX_P0);    // Enable pipe 0

    
    nrf24_wreg(FEATURE, 0);  // Enable dynamic package length
    nrf24_wreg(DYNPD, 0);    // Enable dynamic package length for pipe 0

    /*
    spi_buffer_size = 2;
    spi.buffer[0] = CMD_ACTIVATE;
    spi.buffer[1] = 0x73;
    spi_swap();
    
    for (long int i = 0; i < 10000; i++) {}
    nrf24_wreg(FEATURE, FEATURE_EN_DPL);  // Enable dynamic package length

    for (long int i = 0; i < 10000; i++) {}
    nrf24_wreg(DYNPD, DYNPD_0);    // Enable dynamic package length for pipe 0
     */
    
    spi_buffer_size = 5;

    spi.buffer[0] = CMD_W_REGISTER | (REGISTER_MASK & RX_ADDR_P0);
    spi.buffer[1] = (HA_ADDRESS_MY >> 0) & 0xFF;
    spi.buffer[2] = (HA_ADDRESS_MY >> 8) & 0xFF;
    spi.buffer[3] = (HA_ADDRESS_MY >> 16) & 0xFF;
    spi.buffer[4] = (HA_ADDRESS_MY >> 24) & 0xFF;
    spi_send();
    
    spi.buffer[0] = CMD_W_REGISTER | (REGISTER_MASK & TX_ADDR);
    spi.buffer[1] = (HA_ADDRESS_MASTER >> 0) & 0xFF;
    spi.buffer[2] = (HA_ADDRESS_MASTER >> 8) & 0xFF;
    spi.buffer[3] = (HA_ADDRESS_MASTER >> 16) & 0xFF;
    spi.buffer[4] = (HA_ADDRESS_MASTER >> 24) & 0xFF;
    spi_send();
}


char nrf24_send_msg() {
    nrf24_print();

    POWER_OFF;
    
    nrf24_wreg(RF_CH, 103);
    
/*
    log_printf(">>> SENDING... ");
    for (unsigned char i = 0; i < 32; i++) {
        log_printf(" [%x]", spi.buffer[i]);
    }
    log_printf("\r\n");
 */
    
    
    POWER_SEND;
    for (long int i = 0; i < 100000; i++) {}
    
 
    SPI_SLAVE_ON;
    
    SSP1BUF = CMD_W_TX_PAYLOAD;
    SEND_SSP1BUF

    for (unsigned char i = 0; i < 32; i++) {
        SSP1BUF = spi.buffer[i];
        SEND_SSP1BUF
    }
    SPI_SLAVE_OFF;
    spi_buffer_size = 1;

    while (1) {
        spi.buffer[0] = CMD_NOP;
        spi_swap();
        if (spi.buffer[0] & STATUS_MAX_RT) {
            // Error
            log_printf("Error!!");
            nrf24_wreg(STATUS, STATUS_MAX_RT);
            CE_OFF;
            POWER_OFF;
            return 0;
        }
        if (spi.buffer[0] & STATUS_TX_DS) {
            // Success
            log_printf("Success!!");
            nrf24_wreg(STATUS, STATUS_TX_DS);
            CE_OFF;
            POWER_OFF;
            return 1;
        }
        log_printf("(SET CE_ON)");
        CE_ON;
        
        //spi_buffer_size = 2;
        //spi.buffer[0] = OBSERVE_TX;
        //spi.buffer[1] = 0;
        //spi_swap();
        //log_printf("STATISTICS: Lost: %i, Retransmitted: %i", spi.buffer[1] >> 3, spi.buffer[1] & 7);
        
        //for (long int i = 0; i < 10000; i++) {}
    }
    return 0;
}

void nrf24_start_quick_com() {
    log_printf("Starting com...");

    POWER_OFF;
    nrf24_configure_for_start_com();
    
    memset(&spi, 0, sizeof(spi));
    spi.air_msg.msg_type = MSG_TYPE_START_COM;
    spi.air_msg.msg_start_com.address_my = HA_ADDRESS_MY_LOCAL;
    spi.air_msg.msg_start_com.device_uid = 0x12345678;

    // TODO Calc spi.air_msg.checksum
    nrf24_send_msg();
        
    nrf24_configure_quick_com();
    for (long int i = 0; i < 10000; i++) {}

    POWER_RECIEVE;

    CE_ON;

    TMR4 = 0;
    uint8_t wait = 0;
    while (1) {
        SPI_SLAVE_ON
        SSP1BUF = CMD_NOP;
        while (!SSP1STATbits.BF) {}
        spi.buffer[0] = SSP1BUF;
        SPI_SLAVE_OFF
                
        if (spi.buffer[0] & STATUS_RX_DR) {
            // Success
            log_printf("Get packet!! (");
            nrf24_wreg(STATUS, STATUS_RX_DR);
            
            nrf24_rreg(RX_PW_P0);
            spi_buffer_size = SSP1BUF;
            log_printf("%u bytes)\r\n", spi_buffer_size);
            
            SPI_SLAVE_ON;

            SSP1BUF = CMD_R_RX_PAYLOAD;
            SEND_SSP1BUF

            for (unsigned char i = 0; i < spi_buffer_size; i++) {
                SSP1BUF = 0;
                SEND_SSP1BUF;
                spi.buffer[i] = SSP1BUF;
            }
            for (unsigned char i = spi_buffer_size; i < SPI_BUFFER_MAX_SIZE; i++) {
                spi.buffer[i] = 0;
            }
            
            SPI_SLAVE_OFF;

            log_printf("=======> RESPONSE: Channel: %u, MsgId: %u\r\n",
                    spi.air_msg.msg_start_com_resp.channel,
                    spi.air_msg.msg_start_com_resp.msg_id);
            

            return;
        }
        if (PIR3bits.TMR4IF) {
            PIR3bits.TMR4IF = false;
            if (wait++ > 20) {
                break;
            }
        }
        
        //spi_buffer_size = 2;
        //spi.buffer[0] = OBSERVE_TX;
        //spi.buffer[1] = 0;
        //spi_swap();
        //log_printf("STATISTICS: Lost: %i, Retransmitted: %i", spi.buffer[1] >> 3, spi.buffer[1] & 7);
        
        //for (long int i = 0; i < 10000; i++) {}
    }
 

    CE_OFF;
}


void nrf24_set_channel(uint8_t channel) {
    nrf24_wreg(RF_CH, channel);
}
