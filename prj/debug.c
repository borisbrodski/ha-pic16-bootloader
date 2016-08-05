#define DEBUG_C
#include "common.h"

#ifdef LOG_ACTIVE


unsigned char dbg_byte_to_write;
unsigned char dbg_send_byte_b;
unsigned char dbg_send_byte_i;

void dbg_send_byte();

inline void log_char(const uint8_t c) {
    dbg_byte_to_write = c;
    dbg_send_byte();
}

void log_string(const uint8_t * msg) {
    int i;
    for (i = 0; msg[i]; i++) {
        log_char(msg[i]);
    }
}

void log_buffer(const uint8_t * buf, uint8_t length) {
    uint8_t i;
    char hexbuf[3];

    for (i = 0; i < length; i++) {
        utoa(hexbuf, 0xFF & buf[i], 16);
        if (!hexbuf[1]) {
            log_char('0');
        }
        log_string(hexbuf);
        log_char(' ');
    }
    log_string("    ");
    for (i = 0; i < length; i++) {
        uint8_t c = buf[i];
        if (c >= 32 && !(c & 0x80)) {
            log_char(c);
        } else if (c) {
            log_char('#');
        } else {
            log_char('.');
        }
    }
}

void log_vprintf(const char * fmt, va_list args) {
    int i;
    unsigned char buf[10];
    for (i = 0; fmt[i]; i++) {
        if (fmt[i] == '%' && fmt[i + 1]) {
            i++;
            switch (fmt[i]) {
                case 'u': {
                    unsigned int pi = va_arg(args, unsigned int);
                    utoa(buf, pi, 10);
                    log_string(buf);
                    break;
                }
                case 'x': {
                    unsigned int pi = va_arg(args, unsigned int);
                    utoa(buf, pi, 16);

                    log_string(buf);
                    break;
                }
                case 's': {
                    unsigned char * str = va_arg(args, unsigned char*);
                    log_string(str);
                    break;
                }
                case 'c': {
                    unsigned int c = va_arg(args, unsigned int);
                    log_char((uint8_t)(0xFF & c));
                    break;
                }
                case 'b': {
                    uint8_t * pbuf = va_arg(args, uint8_t *);
                    unsigned int length = va_arg(args, unsigned int);
                    log_buffer(pbuf, length);
                    break;
                }
            }
        } else {
            log_char(fmt[i]);
        }
    }
}

void log_printf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_vprintf(fmt, args);
    va_end(args);
}

#endif