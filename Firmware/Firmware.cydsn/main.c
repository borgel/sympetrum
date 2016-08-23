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
#include "beacon.h"
#include "systime.h"


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
    beacon_Start(id);
    
    patterns_Start();
    
    debprint("Board ID = 0x%x\r\n", id);
    
    //without debounce this sucks
    //ButtonInterrupt_StartEx(ButtonISR);
    
    for(;;) {
        ir_GiveTime();
        beacon_GiveTime();
        patterns_GiveTime();
    }
}
