/*
*/
#include <project.h>
#include <stdint.h>

#include "debprint.h"

int main()
{
    CyGlobalIntEnable;
    
    debprint_Start();
    
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
        CyDelay(500);
    }
}

/* [] END OF FILE */
