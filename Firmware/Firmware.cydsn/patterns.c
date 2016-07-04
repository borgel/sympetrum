#include "patterns.h"
#include "leds.h"
#include "rng.h"
#include "debprint.h"

#include <project.h>

#define MAX_TENDENCY                    2
#define ITERATIONS_UNTIL_CHANGE         10  //30Hz framerate
#define ITERATIONS_UNTIL_TENDENCIES     10

/*
color: The current color to display at this LED.
tendency: The rate at which 
*/
struct pattern_AnimationState {
    struct led_PackedColor color;
    int8_t tendency;
};
struct pattern_AnimationState animation[LED_CHAIN_LENGTH] = {};

struct pattern_State {
    int untilNextChange;
    int untilNextTendencies;
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
    unsigned int i;
    
    //periodically change the fade directions of the LEDs
    if(state.untilNextChange == 0) {
        state.untilNextChange = ITERATIONS_UNTIL_CHANGE;
        
        //TODO do pattern stuff
        for(i = 0; i < sizeof(animation); i++) {
            //if(state.untilNextTendencies == 0)
            {
                state.untilNextTendencies = ITERATIONS_UNTIL_TENDENCIES;
                
                if(rng_IsCoinHeads()) {
                    animation[i].tendency++;
                }
                else {
                    animation[i].tendency--;
                }
                
                //clamp the tendency
                if(animation[i].tendency > MAX_TENDENCY) {
                    animation[i].tendency = MAX_TENDENCY;
                }
                else if(animation[i].tendency < -MAX_TENDENCY) {
                    animation[i].tendency = -MAX_TENDENCY;
                }
                
                //TODO if a channel is railed, reverse its tendency
            }
            
            //TODO check if < min or > max and clamp
            
            //apply the tendency to all color channels
            animation[i].color.r += animation[i].tendency;
            animation[i].color.g += animation[i].tendency;
            animation[i].color.b += animation[i].tendency;
        }
    }
    
    for(i = 0; i < sizeof(animation); i++) {
        led_SetColor(i, &animation[i].color);
    }
    
    state.untilNextChange--;
    state.untilNextTendencies--;
}
