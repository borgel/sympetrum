#include "patterns.h"
#include "leds.h"
#include "rng.h"
#include "debprint.h"

#include <project.h>

struct pattern_State {
};
static const struct pattern_State stateNull = {};
static struct pattern_State state;

void pattern_PermutePattern(void);

/*
Every time the new frame interrupt fires, display whatever is in the LED buffer
*/
CY_ISR(led_FrameISR) {
    pattern_PermutePattern();
    led_DisplayPattern();
    
    RGBFrameTimer_ClearInterrupt(RGBFrameTimer_INTR_MASK_TC);
    RGBFrameInterrupt_ClearPending();
}

void patterns_Start(void) {
    state = stateNull;
    
    //setup the frame interrupt and timer
    RGBFrameTimer_Start();
    RGBFrameInterrupt_StartEx(led_FrameISR);
}

/*
Be clever
*/
void pattern_PermutePattern(void) {
    //set some LEDs
    struct led_PackedColor c = {0, 0, 0};
    
    c.b = rng_GetByte()/9;
    led_SetColor(0, &c);
    c.b = 0;
    
    c.r = rng_GetByte()/9;
    led_SetColor(1, &c);
    c.r = 0;
    
    c.g = rng_GetByte()/9;
    led_SetColor(2, &c);
    c.g = 0;
}
