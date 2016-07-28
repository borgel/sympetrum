#include "beacon.h"
#include "systime.h"
#include "ir.h"
#include "debprint.h"
#include <project.h>

#define         BEACON_INTERVAL_S   (1000 * 15)

struct beacon_State {
    systime_t lastBeacon;
    struct ir_Message beaconMsg;
};
static struct beacon_State state = {};

void beacon_Start(uint8_t boardID) {
    state.lastBeacon = 0;
    state.beaconMsg.body = boardID;
    
    //make sure IR knows what to ignore
    ir_SetBeaconToIgnore(&state.beaconMsg);
}

static void beacon_Send(void) {
    ir_Send(&state.beaconMsg);
}

void beacon_GiveTime(void) {
    if(systime_GetTimeMS() - state.lastBeacon > BEACON_INTERVAL_S) {
        beacon_Send();
        state.lastBeacon = systime_GetTimeMS();
        
        //UART_Debug_UartPutChar('.');
    }
}
