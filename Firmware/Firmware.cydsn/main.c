/*
*/
#include <project.h>
#include <stdint.h>

#include "board_id.h"
#include "debprint.h"
#include "ir.h"
#include "leds.h"
#include "rng.h"
#include "patterns.h"

int main()
{
    CyGlobalIntEnable;
    
    debprint_Start();
    ir_Start();
    led_Start();
    
    debprint("\r\nStarting "__DATE__ ": "__TIME__"\r\n");
    
    uint8_t id = bid_GetID();
    rng_Start(id);
    patterns_Start();
    
    debprint("Board ID = 0x%x\r\n", id);
    
    uint8_t count = 0;
    
    struct ir_Message msg;
    
    for(;;) {
        /*
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
        */
        
        
        count++;
        CyDelay(50);
    }
}
