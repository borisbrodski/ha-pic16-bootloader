/* 
 * File:   spi.h
 * Author: Boris Brodski
 *
 * Created on July 7, 2016, 9:48 AM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#ifdef SPI_C
    #define SPI_EXTERN
#else
    #define SPI_EXTERN extern
#endif

#define SPI_SLAVE_ON    {PORTCbits.RC3 = 0;}
#define SPI_SLAVE_OFF   {PORTCbits.RC3 = 1;}
#define SPI_SLAVE_INIT  {SPI_SLAVE_OFF; TRISCbits.TRISC3 = 0;}

#define SPI_BUFFER_MAX_SIZE 16

#define MSG_TYPE_START_COM              1
#define MSG_TYPE_START_COM_RESP         2
#define MSG_TYPE_CMD                    3
    
SPI_EXTERN union {
    uint8_t buffer[SPI_BUFFER_MAX_SIZE];
    struct {
        uint8_t msg_type;
        union {
            uint8_t data[SPI_BUFFER_MAX_SIZE - 2 - 4];
            struct {
                uint16_t address_my;
                uint32_t device_uid;
            } msg_start_com;
            struct {
                uint32_t msg_id;
                uint8_t channel;
            } msg_start_com_resp;
        };
        uint32_t checksum;
    } air_msg;
} spi;
SPI_EXTERN uint8_t spi_buffer_size;

void spi_init();

void spi_swap();
void spi_send();

//uint8_t spi_swap_byte(uint8_t b);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

