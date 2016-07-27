#include "systime.h"

#include <project.h>

//#define USE_WDT_SYSCLK

static systime_t time;

//TODO only tick on rollover, and check timer's value every get time
CY_ISR(SystimeTickCB) {
#ifdef USE_WDT_SYSCLK
    //cleared in sys interrupt handler?
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
#else
    SystimeTimer_ClearInterrupt(SystimeTimer_INTR_MASK_TC);
    SystimeTickInterrupt_ClearPending();
#endif//USE_WDT_SYSCLK

    time++;    
}

void systime_Start(void) {
#ifdef USE_WDT_SYSCLK
    //configure watchdog to act as a time
    CySysWdtSetMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
    CySysWdtSetMatch(CY_SYS_WDT_COUNTER0, 32);
    CySysWdtSetClearOnMatch(CY_SYS_WDT_COUNTER0, 1);
    CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER0, systimeCB);
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    CySysWdtDisable(CY_SYS_WDT_COUNTER1_MASK | CY_SYS_WDT_COUNTER1_MASK);
    while(!CySysWdtGetEnabledStatus(CY_SYS_WDT_COUNTER0)) {}
#else
    SystimeClock_Start();
    SystimeTickInterrupt_StartEx(SystimeTickCB);
    SystimeTimer_Start();
#endif//USE_WDT_SYSCLK
}

systime_t systime_GetTimeMS(void){
    return time;
}