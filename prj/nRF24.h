/* 
 * File:   nRF24.h
 * Author: boris
 *
 * Created on July 8, 2016, 8:08 AM
 */

#ifndef NRF24_H
#define	NRF24_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CE_ON    {PORTCbits.RC5 = 1;}
#define CE_OFF   {PORTCbits.RC5 = 0;}
#define CE_INIT  {CE_OFF; TRISCbits.TRISC5 = 0;}
    
#define HA_SUBNET 0x2fd0b000
#define HA_ADDRESS_MY_LOCAL 0xBB0
#define HA_ADDRESS_MASTER   (HA_SUBNET | 0x555)
#define HA_ADDRESS_MY       (HA_SUBNET | HA_ADDRESS_MY_LOCAL)

#define NRF24_CHANNEL_MIN   103
#define NRF24_CHANNEL_MAX   110
    
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
    
void nrf24_init();
void nrf24_set_channel(uint8_t channel);

void nrf24_start_quick_com();
bool nrf24_start_quick_com_resp();



#ifdef	__cplusplus
}
#endif

#endif	/* NRF24_H */

