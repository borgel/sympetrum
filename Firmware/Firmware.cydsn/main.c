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
#include "systime.h"

//#define BEACON

//TODO move
static bool docolor = false;
CY_ISR(ButtonISR) {
    //debprint("Got a button\n");
    
    docolor = true;
    ButtonInterrupt_ClearPending();
}


int main()
{
    systime_Start();
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
    
    //static const struct ir_Message beaconMsg = {.body = id};
    static const struct ir_Message beaconMsg = {.body = 0x3CA5};
    
    for(;;) {
#ifdef BEACON
        if(count % 100 == 0) {
            //debprint("sending body 0x%X data 0x%0X\r\n", beaconMsg.body);
            
            ir_Send(&beaconMsg);
            UserLED_Write(!UserLED_Read());
        }
        CyDelay(12);
#endif

#ifndef BEACON
        ir_GiveTime();
#endif
        
        count++;
    }
}
