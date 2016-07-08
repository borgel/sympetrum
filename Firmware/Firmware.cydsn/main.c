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

//#define BEACON

//TODO move
static bool docolor = false;
CY_ISR(ButtonISR) {
    debprint("Got a button\n");
    
    docolor = true;
    ButtonInterrupt_ClearPending();
}

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
    
    uint8_t count, send;
    count = send = 0;
    
    //without debounce this sucks
    //ButtonInterrupt_StartEx(ButtonISR);
    
    struct ir_Message beaconMsg = {.address = id};
    
    for(;;) {
#ifdef BEACON
        if(count % 100 == 0) {
            beaconMsg.command = send++;
            
            //beaconMsg.address = 0x15;   //0b1 0101
            //beaconMsg.command = 0x0C;   //0b00 1100
            beaconMsg.address = 0x1F;
            beaconMsg.command = 0x3F;
            
            debprint("sending addr %d data 0x%0X\r\n", beaconMsg.address, beaconMsg.command);
            
            ir_Send(&beaconMsg);
            UserLED_Write(!UserLED_Read());
        }
        CyDelay(10);
#endif

#ifndef BEACON
        ir_GiveTime();
#endif
        
        count++;
    }
}
