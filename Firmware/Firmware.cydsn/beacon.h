#ifndef BEACON_H__
#define BEACON_H__
    
#include <stdint.h>
#include "ir.h"
#include "systime.h"
    
void beacon_Start(uint8_t boardID);
void beacon_GiveTime(void);
void beacon_RecordSeenBeacon(struct ir_Message const* beacon, systime_t stamp);
    
#endif//BEACON_H__
