#include "beacon.h"
#include "systime.h"
#include "ir.h"
#include "debprint.h"
#include "rng.h"
#include <project.h>

#define BEACON_INTERVAL_S           (1000 * 15)
#define BEACON_TABLE_SIZE           10

struct beacon_State {
    systime_t lastBeacon;
    struct ir_Message beaconMsg;
};
static struct beacon_State state = {};

static struct beacon_State SeenBeacons[BEACON_TABLE_SIZE];

void beacon_Start(uint8_t boardID) {
    state.lastBeacon = 0;
    state.beaconMsg.body = boardID;
    
    //make sure IR knows what to ignore
    ir_SetBeaconToIgnore(&state.beaconMsg);
    
    memset(SeenBeacons, 0, sizeof(SeenBeacons) * BEACON_TABLE_SIZE);
}

/*
Replace oldest seen beacon with this one.
*/
void beacon_RecordSeenBeacon(struct ir_Message const* beacon, systime_t stamp) {
    //iterate through table and replace oldest with this beacon
    systime_t oldest = 0 - 1;
    int oldestIndex = 0;
    
    struct beacon_State *b;
    int i;
    for(i = 0; i < BEACON_TABLE_SIZE; i++) {
        b = &SeenBeacons[i];
        
        //FIXME do we want this? it works fine without it right?
        //if we've seen this before, always update instead of append
        if(b->beaconMsg.body == beacon->body) {
            oldestIndex = i;
            break;
        }
        
        if(b->lastBeacon < oldest) {
            oldest = b->lastBeacon;
            oldestIndex = i;
        }
    }
    
    //whatever is at oldestIndex is the victim, replace it
    b = &SeenBeacons[oldestIndex];
    b->lastBeacon = stamp;
    b->beaconMsg = *beacon;
    
    debprint("Inserted at index %d, age %d\r\n", oldestIndex, b->lastBeacon);
}

static void beacon_Send(void) {
    ir_Send(&state.beaconMsg);
}

void beacon_GiveTime(void) {
    if(systime_GetTimeMS() - state.lastBeacon > BEACON_INTERVAL_S) {
        beacon_Send();
        
        //throw in a little jitter in the sub-second range
        state.lastBeacon = systime_GetTimeMS() - (4 * rng_GetByte());
    }
}
