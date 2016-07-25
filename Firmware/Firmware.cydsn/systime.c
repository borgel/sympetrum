#include "systime.h"

#include <project.h>

static systime_t time;

CY_ISR(systimeCB) {
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
    time++;
    
    //FIXME rm
    UserLED_Write(!UserLED_Read());
}

void systime_Start(void) {
    //configure watchdog to act as a time
    CySysWdtSetMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
    CySysWdtSetMatch(CY_SYS_WDT_COUNTER0, 32);
    CySysWdtSetClearOnMatch(CY_SYS_WDT_COUNTER0, 1);
    CySysWdtSetInterruptCallback(0, systimeCB);
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    while(!CySysWdtGetEnabledStatus(CY_SYS_WDT_COUNTER0)) {}
}

systime_t systime_GetTimeMS(void){
    return time;
}