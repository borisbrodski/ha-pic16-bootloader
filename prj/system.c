/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations and to compiler documentation for macro details. */
inline void ConfigureOscillator(void)
{

    /*If the PIC12 device has an OSCCAL value, the HiTech Compiler provides
    a macro called _READ_OSCCAL_DATA which can be loaded using this: */

    /* TODO Configure OSCCAL if the device has an OSCCAL register */

#if 0

    OSCCAL=_READ_OSCCAL_DATA(); /* _READ_OSCCAL_DATA macro unloads cal memory */

#endif

    //OSCCON.IRFC<3:0> = 1101 // 4 MHz
    //OSCCON.IRFC<3:0> = 1110 // 8 MHz
    //OSCCON.IRFC<3:0> = 1111 // 16 MHz
    OSCCONbits.IRCF = 0b00001110;
    
    /*Not all PIC12 devices require this. */

    /* TODO Add clock switching code if appropriate.  */

    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */

}
