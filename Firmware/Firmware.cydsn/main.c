/*
*/
#include <project.h>
#include <stdint.h>

#include "debprint.h"
#include "ir.h"

int main()
{
    CyGlobalIntEnable;
    
    debprint_Start();
    ir_Start();
    
    IR_Transmit_Start();
    PWM_1_Start();
    
    uint8_t count = 0;
    
    uint16_t data;
    for(;;) {
        msg.address = 5;
        msg.command = 33;
        ir_Send(&msg);
        //debprint("sending addr %d data 0x%0X\r\n", msg.address, msg.command);
        
        if(ir_Receive(&msg)) {
            debprint("received addr %d data 0x%0X\r\n", msg.address, msg.command);
        }
        else {
            //debprint("Failed to rx a message\n");
        }
        
        count++;
        CyDelay(500);
    }
}

/* [] END OF FILE */
