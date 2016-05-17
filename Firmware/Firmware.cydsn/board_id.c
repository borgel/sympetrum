#include "board_id.h"
#include <project.h>

uint8_t bid_GetID(void) {
    uint8_t id;
    
    //set straps to high so we can read which are connected
    BoardIDStraps_Write(0xFF);
    
    id = BoardIDStraps_Read();
    
    //set straps to low to save power
    BoardIDStraps_Write(0x0);
    
    return id;
}
