#ifndef SYSTIME_H__
#define SYSTIME_H__
    
#include <stdint.h>
    
//rolls over every ~49 days
typedef uint32_t systime_t;
    
void systime_Start(void);
systime_t systime_GetTimeMS(void);
    
#endif//SYSTIME_H__