#include "patterns.h"
#include "leds.h"
#include "rng.h"
#include "color.h"

#include <project.h>

#define FRAMES_TO_CHANGE_TARGET     40  //10 = 1 seconds

struct pattern_AnimationState {
    int channel;
    struct color_PackedColor colorTarget;
    struct color_PackedColor colorCurrent;
    uint8_t stepMagnitude;
};
struct pattern_AnimationState animation[LED_CHAIN_LENGTH] = {};

struct pattern_State {
    bool permutePattern;
    int framesSinceTargetChange;
};
static const struct pattern_State stateNull = {
    .permutePattern = false,
    .framesSinceTargetChange = 0
};
static struct pattern_State state;

void pattern_PermutePattern(void);

/*
10Hz animation update ISR
*/
CY_ISR(animation_FrameISR) {
    state.permutePattern = true;
    
    AnimationTimer_ClearInterrupt(AnimationTimer_INTR_MASK_TC);
    AnimationFrameInterrupt_ClearPending();
}

void patterns_Start(void) {
    state = stateNull;
    
    //set random color targets for all channels
    unsigned int i;
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        //explicitely associate all channels
        animation[i].channel = i;
        
        color_GetRandomColor(&animation[i].colorTarget);
        
        //TODO set randomly within range
        animation[i].stepMagnitude = 2;
    }
    
    //setup the frame interrupt and timer
    AnimationTimer_Start();
    AnimationFrameInterrupt_StartEx(animation_FrameISR);
}

//compare current color to target, and step it towards the target accordingly
static void compareStepColorChannel(uint8_t *current, uint8_t target, uint8_t step) {
    if(*current < target) {
        *current += step;
    }
    else if (*current > target) {
        *current -= step;
    }
}

/*
Be clever
*/
void pattern_PermutePattern(void) {
    unsigned int i;
    
    if(state.framesSinceTargetChange >= FRAMES_TO_CHANGE_TARGET) {
        for(i = 0; i < sizeof(animation); i++) {
            color_GetRandomColor(&animation[i].colorTarget);
        }
        state.framesSinceTargetChange = 0;
    }
    
    //step all channels towards their target
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        //figure out which direction to step in, then do that
        compareStepColorChannel(&animation[i].colorCurrent.r, animation[i].colorTarget.r, animation[i].stepMagnitude);
        compareStepColorChannel(&animation[i].colorCurrent.g, animation[i].colorTarget.g, animation[i].stepMagnitude);
        compareStepColorChannel(&animation[i].colorCurrent.b, animation[i].colorTarget.b, animation[i].stepMagnitude);
    }
    
    //copy the pattern out to be displayed
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        led_SetColor(animation[i].channel, &animation[i].colorCurrent);
    }
    
    state.framesSinceTargetChange++;
}

void patterns_GiveTime(){
    if(state.permutePattern) {
        pattern_PermutePattern();
        state.permutePattern = false;
    }
}
