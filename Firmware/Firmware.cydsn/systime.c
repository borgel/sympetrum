#include "systime.h"

#include <project.h>

// beacon age math is easier if we pretend to never be younger then 1 minute
#define SYSTIME_START       (60 * 1000)

#define TIMER_ROLLOVER  65535
static systime_t time;

CY_ISR(SystimeTimerRollover) {
    SystimeTimer_ClearInterrupt(SystimeTimer_INTR_MASK_TC);
    SystimeTickInterrupt_ClearPending();

    time += TIMER_ROLLOVER;    
}

void systime_Start(void) {
    time = SYSTIME_START;
    
    SystimeClock_Start();
    SystimeTickInterrupt_StartEx(SystimeTimerRollover);
    SystimeTimer_Start();
}

systime_t systime_GetTimeMS(void){
    uint8_t flags = CyEnterCriticalSection();
    systime_t t = time + SystimeTimer_ReadCounter();
    CyExitCriticalSection(flags);
    
    return t;
}