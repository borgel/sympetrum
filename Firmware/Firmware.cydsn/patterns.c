#include "patterns.h"
#include "leds.h"
#include "rng.h"
#include "debprint.h"
#include "beacon.h"
#include "color.h"

#include <math.h>
#include <project.h>

#define FRAMES_TO_CHANGE_TARGET     40  //10 = 1 seconds
//2 is a gentle default. 5 is pretty fast
#define ANIMATION_STEP_SIZE         1

struct pattern_AnimationState {
    int channel;
    struct color_ColorHSV colorTarget;
    struct color_ColorHSV colorCurrent;
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
        
        color_GetRandomColorH(&animation[i].colorTarget);
        animation[i].colorCurrent = COLOR_HSV_MAXSV;
        
        //TODO set randomly within range
        animation[i].stepMagnitude = ANIMATION_STEP_SIZE;
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
        uint8_t avgColor;
        float tableFullness = beacon_GetTableData(&avgColor);
        for(i = 0; i < LED_CHAIN_LENGTH; i++) {
            color_GetRandomColorH(&animation[i].colorTarget);
            
            //TODO incorperate visible beacons in targets
            
            //v0 progress to hue of visible badge IDs
            animation[i].colorTarget.h = avgColor;
            
            //TODO use a ratio fo the rng color and table color
            
        }
        
        state.framesSinceTargetChange = 0;
    }
    
    //step all channels towards their target
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        //figure out which direction to step in, then do that
        compareStepColorChannel(&animation[i].colorCurrent.h, animation[i].colorTarget.h, animation[i].stepMagnitude);
    }
    
    //copy the pattern out to be displayed
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        led_SetColorHSV(animation[i].channel, &animation[i].colorCurrent);
    }
    
    state.framesSinceTargetChange++;
}

void patterns_GiveTime(){
    if(state.permutePattern) {
        pattern_PermutePattern();
        state.permutePattern = false;
    }
}
