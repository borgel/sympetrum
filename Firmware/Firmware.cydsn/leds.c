/*
*/
/*
Notes on dotstars:
https://cpldcpu.wordpress.com/2014/08/27/apa102/
people have seen global brightness below (30?) cause flickering, so ignore it
someone pointed out endframe is no different looking then a full bright LED (which
sort of matches behavior I've seen....). So omit that too.

*/
#include "leds.h"
#include "color.h"
#include <project.h>

#include <stdint.h>

//the threshold below which the LEDs begin to flicker
#define LED_FLICKER_THRESHOLD                   3

static const uint32_t   LED_START_FRAME        = 0x00000000;
static const uint32_t   LED_END_FRAME          = 0xFFFFFFFF;
static const uint8_t    LED_MAX_SAFE_BRIGHT    = 255;    //cap brightness at ~10%

struct led_data {
    //header/global brightness is 0bAAABBBBB
    //A = 1
    //B = integer brightness divisor from 0x0 -> 0x1F
    uint8_t       globalHeader;
    struct color_ColorRGB color;
};

static struct led_data LedState[LED_CHAIN_LENGTH] = {};

static const struct led_data led0 = {0xE4, {0, 0, 0}};

/*
Every time the new frame interrupt fires, display whatever is in the LED buffer
*/
CY_ISR(led_FrameISR) {
    led_DisplayPattern();
    
    RGBFrameTimer_ClearInterrupt(RGBFrameTimer_INTR_MASK_TC);
    RGBFrameInterrupt_ClearPending();
}

void led_Start(void) {
    SPI_LED_Start();
    
    //setup all LED states (header + global to max = led0)
    uint8_t i;
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        LedState[i] = led0;
    }
    
    //0 all LEDs
    led_DisplayPattern();
    
    //setup the frame interrupt and timer
    RGBFrameTimer_Start();
    RGBFrameInterrupt_StartEx(led_FrameISR);
}

/*
Play whatever is in the current state buffer out
*/
void led_DisplayPattern(void) {
    int i;
    
    //start frame
    SPI_LED_SpiUartWriteTxData(0x00);
    SPI_LED_SpiUartWriteTxData(0x00);
    SPI_LED_SpiUartWriteTxData(0x00);
    SPI_LED_SpiUartWriteTxData(0x00);
    
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        SPI_LED_SpiUartWriteTxData(LedState[i].globalHeader);
        SPI_LED_SpiUartWriteTxData(LedState[i].color.b);
        SPI_LED_SpiUartWriteTxData(LedState[i].color.g);
        SPI_LED_SpiUartWriteTxData(LedState[i].color.r);
    }
    
    SPI_LED_SpiUartWriteTxData(0xFF);
    SPI_LED_SpiUartWriteTxData(0xFF);
    SPI_LED_SpiUartWriteTxData(0xFF);
    SPI_LED_SpiUartWriteTxData(0xFF);
}

void led_SetColor(int index, struct color_ColorRGB *const color) {
    if(index > LED_CHAIN_LENGTH || !color) {
        return;
    }
    
    //cap each channel's brightness at something sane
    if(color->r > LED_MAX_SAFE_BRIGHT) {
        color->r = LED_MAX_SAFE_BRIGHT;
    }
    color->r = (color->r < LED_FLICKER_THRESHOLD) ? (color->r * 3) : color->r;
    
    if(color->g > LED_MAX_SAFE_BRIGHT) {
        color->g = LED_MAX_SAFE_BRIGHT;
    }
    color->g = (color->g < LED_FLICKER_THRESHOLD) ? (color->g * 3) : color->g;
    
    if(color->b > LED_MAX_SAFE_BRIGHT) {
        color->b = LED_MAX_SAFE_BRIGHT;
    }
    color->b = (color->b < LED_FLICKER_THRESHOLD) ? (color->b * 3) : color->b;
    
    LedState[index].color = *color;
}
