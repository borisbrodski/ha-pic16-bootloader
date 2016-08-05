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

    
    
#define HA_SUBNET 0x2fd0b000
#define HA_ADDRESS_MY_LOCAL 0xBB0
#define HA_ADDRESS_MASTER   (HA_SUBNET | 0x555)
#define HA_ADDRESS_MY       (HA_SUBNET | HA_ADDRESS_MY_LOCAL)




#ifdef	__cplusplus
}
#endif

#endif	/* NRF24_COM_H */

