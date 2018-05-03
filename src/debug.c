#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "debug.h"

#define DEBUG_USART_PORT  0
#define UsartSendPolling(n, str, count) printf(str)

#define DEBUG_HEX_LINE_WIDTH 16
static char g_printHexBuf[32];
static void _print_hex(const unsigned char* data, int len)
{
	int i;
    int prtlen;

	for (i = 0; i < DEBUG_HEX_LINE_WIDTH; ++i){
		if (i < len){
            prtlen = sprintf(g_printHexBuf, "%02X ", data[i]);
            UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)g_printHexBuf, prtlen);
		} else {
		    UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)"   ", 3);
		}
	}

	return;
}

static void _print_ascii(const unsigned char* data, int len)
{
	int i;
    int prtlen;

	for (i = 0; i < len; ++i){
		if (data[i] >= 32 && data[i] <= 126){
            prtlen = sprintf(g_printHexBuf, "%c", data[i]);
            UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)g_printHexBuf, prtlen);
		} else {
            UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)".", 1);
		}
	}
}

void _DebugPrintHex(const unsigned char* buf, int buflen)
{
	int i;
	int len;
    int prtlen;
    
	for (i = 0; i < buflen;){

		len = buflen - i;
		if (len > 16){
			len = 16;
		}

        prtlen = sprintf(g_printHexBuf, "0x%08X  ", i);
        UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)g_printHexBuf, prtlen);
	    _print_hex(buf + i, len);
        UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)"  ", 2);
		_print_ascii(buf + i, len);
        UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)"\r\n", 2);
	
		i += DEBUG_HEX_LINE_WIDTH;
	}
}

//输出调试信息
void _DebugPrintf(const char* fmt, ...)
{
	char buf[100];
	va_list ap;
	int len;

	memset(buf, 0, sizeof(buf));
	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    if (len > 0){
        UsartSendPolling(DEBUG_USART_PORT, (uint8_t*)buf, len);
    }

    return;
}

void _DebugInit(void)
{   
    return;
}


