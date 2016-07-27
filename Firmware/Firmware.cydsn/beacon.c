#include "beacon.h"
#include "systime.h"
#include "ir.h"
#include "debprint.h"
#include <project.h>

#define         BEACON_INTERVAL_S   (1000 * 15)

struct beacon_State {
    systime_t lastBeacon;
    uint8_t id;
};
static struct beacon_State state = {};

void beacon_Start(uint8_t boardID) {
    state.lastBeacon = 0;
    state.id = boardID;
}

static void beacon_Send(void) {
    const struct ir_Message beaconMsg = {.body = state.id};
    ir_Send(&beaconMsg);
}

void beacon_GiveTime(void) {
    if(systime_GetTimeMS() - state.lastBeacon > BEACON_INTERVAL_S) {
        beacon_Send();
        state.lastBeacon = systime_GetTimeMS();
        
        //UART_Debug_UartPutChar('.');
    }
}
