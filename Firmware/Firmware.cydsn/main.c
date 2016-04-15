/*
*/
#include <project.h>

#include <stdint.h>
#include <stdio.h>



#include <stdarg.h>
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


int main()
{
    CyGlobalIntEnable;
    UART_Debug_Start();
    
    IR_Transmit_Start();
    PWM_1_Start();
    
    uint8_t count = 0;
    
    uint16_t data;
    for(;;) {
        data = count;
        
        //IR_Transmit_WriteTxData(data);
        IR_Transmit_WriteTxData(0x0001);
        
        debprint("a value = %d\r\n", IR_Receiver_Read());
        
        count++;
        CyDelay(10);
    }
}

/* [] END OF FILE */
