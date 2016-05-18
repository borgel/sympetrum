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
    RGBFrameTimer_ClearInterrupt(RGBFrameTimer_INTR_MASK_TC);
    RGBFrameInterrupt_ClearPending();
    
    //TODO permute pattern
    pattern_PermutePattern();
    
    //
    led_DisplayPattern();
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
    struct led_PackedColor c = {0, 5, 0};
    led_SetColor(0, &c);
    c.g = rng_GetByte()/10;
    led_SetColor(2, &c);
}
