/* 
 * File:   nRF24-com.h
 * Author: boris
 *
 * Created on August 3, 2016, 10:41 AM
 */

#ifndef NRF24_COM_H
#define	NRF24_COM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define SPI_SLAVE_PORT  PORTC
#define SPI_SLAVE_TRIS  TRISC
#define SPI_SLAVE_PIN   3

#define CE_PORT PORTC
#define CE_TRIS TRISC
#define CE_PIN  5

// Set maximum amount of nRF24 status
// register reads after which
// the message receive method will
// return an error.
// 0 - 0xFFFF
#define MAX_READ_TRYS 1000
    
    
#define HA_SUBNET 0x2fd0b000
#define HA_ADDRESS_MY_LOCAL 0xBB0
#define HA_ADDRESS_MASTER   (HA_SUBNET | 0x555)
#define HA_ADDRESS_MY       (HA_SUBNET | HA_ADDRESS_MY_LOCAL)


// 4 byte device id; set with -D option
#ifndef DEVICE_ID
  #define DEVICE_ID           0x12345678
#endif


#define MSG_TYPE_START_COM          1
#define MSG_TYPE_START_COM_RESP     2
#define MSG_TYPE_CMD                3

/**
 * Message formats
 * ===============
 *
 * MSG_TYPE_START_COM:
 *
 * | sz | description                  |
 * | 1  | MSG_TYPE_START_COM           |
 * | 2  | My local address, LB first   |
 * | 4  | Device id, LByte first       |
 *
 *
 * MSG_TYPE_START_COM_RESP:
 *
 * | sz | description                  |
 * | 1  | MSG_TYPE_START_COM           |
 * |
 */



/*  
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

*/



#define DYNPD       0x1C
#define DYNPD_0     0b00000001
#define DYNPD_1     0b00000010
#define DYNPD_2     0b00000100
#define DYNPD_3     0b00001000
#define DYNPD_4     0b00010000
#define DYNPD_5     0b00100000
    
#define FEATURE             0x1D
#define FEATURE_EN_DPL      0b00000100
#define FEATURE_EN_ACK_PAY  0b00000010
#define FEATURE_EN_DYN_ACK  0b00000001
    
#define STATUS_MAX_RT_POSN  4
#define STATUS_TX_DS_POSN   5
#define STATUS_RX_DR_POSN   6


#ifdef	__cplusplus
}
#endif

#endif	/* NRF24_COM_H */

