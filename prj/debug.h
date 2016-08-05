/* 
 * File:   debug.h
 * Author: boris
 *
 * Created on July 7, 2016, 10:20 AM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef LOG_ACTIVE
    #define BEGIN_LOG_PROC
    #define END_LOG_PROC        ;
#else
    #ifdef DEBUG_C
        #define BEGIN_LOG_PROC      
        #define END_LOG_PROC        {}
    #else
        #define BEGIN_LOG_PROC
        #define END_LOG_PROC        ;
    #endif
#endif
    
#include <stdint.h>
#include <stdarg.h>

    
BEGIN_LOG_PROC
    inline void log_char(const uint8_t c)
END_LOG_PROC

BEGIN_LOG_PROC
    void log_string(const uint8_t * msg)
END_LOG_PROC

BEGIN_LOG_PROC
    void log_buffer(const uint8_t * buf, uint8_t length)
END_LOG_PROC

BEGIN_LOG_PROC
    void log_vprintf(const char * fmt, va_list args)
END_LOG_PROC

BEGIN_LOG_PROC
    void log_printf(const char * fmt, ...)
END_LOG_PROC


#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

