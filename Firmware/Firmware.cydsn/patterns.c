#include "patterns.h"
#include "leds.h"
#include "rng.h"
#include "debprint.h"
#include "beacon.h"
#include "color.h"
#include "utilities.h"

#include <math.h>
#include <project.h>

#define FRAMES_TO_CHANGE_TARGET     40  //10 = 1 seconds

#define ANIMATION_STEP_SIZE_DEFAULT 1
#define ANIMATION_STEP_SIZE_MAX     6

//give extra importance to the shared beacon average color
#define BEACON_COLOR_WEIGHT_MULTIPLIER      2.0

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
        animation[i].stepMagnitude = ANIMATION_STEP_SIZE_DEFAULT;
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
    struct pattern_AnimationState *a;
    
    if(state.framesSinceTargetChange >= FRAMES_TO_CHANGE_TARGET) {
        uint8_t avgColor;
        float tableFullness = beacon_GetTableData(&avgColor);   
        
        int newStepMagnitude = 0;
        //multiply max animation speed by original fullness. Window between default and max step size
        newStepMagnitude = (float)ANIMATION_STEP_SIZE_MAX * tableFullness;
        newStepMagnitude = MIN(newStepMagnitude, ANIMATION_STEP_SIZE_MAX); 
        newStepMagnitude = MAX(ANIMATION_STEP_SIZE_DEFAULT, newStepMagnitude);
         
        tableFullness *= BEACON_COLOR_WEIGHT_MULTIPLIER;
        
        for(i = 0; i < LED_CHAIN_LENGTH; i++) {
            a = &animation[i];
            color_GetRandomColorH(&a->colorTarget);
            
            //set the target hue to a blend of the RGN value and the table average
            a->colorTarget.h = 
                (tableFullness * (float)avgColor) + 
                ((1.0 - tableFullness) * a->colorTarget.h);
            
            a->stepMagnitude = newStepMagnitude;
            
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
