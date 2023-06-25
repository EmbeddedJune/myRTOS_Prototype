#include "stdint.h"
#include "stdio.h"
#include "HalUart.h"

#define PRINTF_BUF_LEN  1024
static char sPrintf_buf[PRINTF_BUF_LEN];   // 1KiB

uint32_t putstr(const char* s)
{
    uint32_t ret = 0;
    while (*s) {
        Hal_uart_put_char(*s++);
        ret++;
    }
    return ret;
}

uint32_t debug_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(sPrintf_buf, format, args);
    va_end(args);

    return putstr(sPrintf_buf);
}

uint32_t vsprintf(char* buf, const char* format, va_list arg)
{
    uint32_t c = 0;         // length of string
    uint32_t uint, hex;
    char ch;
    char* str;

    for (uint32_t i = 0; format[i]; i++) {
        if (format[i] == '%') {
            switch(format[++i]) {
            case 'c':
                ch = (char)va_arg(arg, int32_t);
                buf[c++] = ch;
                break;
            case 's':
                str = (char*)va_arg(arg, char*);
                // 널포인터 전달되는 경우에 대한 에러처리
                if (str == NULL) str = "(null)";
                while (*str) buf[c++] = (*str++);
                break;
            case 'u':
                uint = (uint32_t)va_arg(arg, uint32_t);
                c += utoa(&buf[c], uint, utoa_dec);
                break;
            case 'x':
                hex = (uint32_t)va_arg(arg, uint32_t);
                c += utoa(&buf[c], hex, utoa_hex);
                break;
            }
        }
        else {
            buf[c++] = format[i];
        }
    }
    // 길이가 PRINTF_BUF_LEN를 넘으면 처리한 문자열 전부 버린다.
    if (c >= PRINTF_BUF_LEN) {
        buf[0] = '\0';
        return 0;
    }
    buf[c] = '\0';
    return c;
}

uint32_t utoa(char* buf, uint32_t val, utoa_t base)
{
    uint32_t c = 0;
    int32_t idx = 0;
    char tmp[11];   // 32-bit 정수는 11자리에 저장 가능하다.

    do {
        uint32_t t = val % (uint32_t)base;
        if (t >= 10) t += ('A' - 10 - '0');
        tmp[idx++] = (t + '0');
        val /= base;
    } while (val);

    idx--;
    while (idx >= 0) buf[c++] = tmp[idx--];
    return c;
}