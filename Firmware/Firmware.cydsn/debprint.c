#include "debprint.h"

#include <stdarg.h>
#include <stdio.h>
#include <project.h>

void debprint_Start() {
    UART_Debug_Start();
}

void debprint(const char *fmt, ...) {
    char printbuf[512] = {0};
    if(!fmt) {
        return;
    }
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(printbuf, sizeof(printbuf), fmt, args);
    va_end(args);
    
    UART_Debug_UartPutString(printbuf);
}
