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
#include "debprint.h"
#include <project.h>

#include <stdint.h>

static const uint32_t   LED_START_FRAME        = 0x00000000;
static const uint32_t   LED_END_FRAME          = 0xFFFFFFFF;
#define                 LED_PACKET_HEADER       0x7     //0b111

struct led_data {
    uint8_t     header  :3;
    uint8_t     global  :5;
    struct led_PackedColor color;
}__attribute__((packed));

static struct led_data LedState[LED_CHAIN_LENGTH] = {};

//0x7 is a constant
//hdr, 5 bit global brightness divisor, each channel
//0x1F is max current
static const struct led_data led50 = {LED_PACKET_HEADER, 0x1F, {255/10, 255/10, 255/10}};
static const struct led_data led0 = {LED_PACKET_HEADER, 0x1F, {0, 0, 0}};

void led_Start(void) {
    SPI_LED_Start();
    
    //setup all LED states (header + global to max = led0)
    uint8_t i;
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        LedState[i] = led0;
    }
    
    //0 all LEDs
    led_DisplayPattern();
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
    
    //TODO LED_CHAIN_LENGTH+1? need to make sure FFs dont make it in
    for(i = 0; i < LED_CHAIN_LENGTH; i++) {
        //1 LED at a time
        SPI_LED_SpiUartPutArray((uint8_t*)&LedState[i], sizeof(LedState[0]));
    }
    
    SPI_LED_SpiUartWriteTxData(0xFF);
    SPI_LED_SpiUartWriteTxData(0xFF);
    SPI_LED_SpiUartWriteTxData(0xFF);
    SPI_LED_SpiUartWriteTxData(0xFF);
}

void led_SetColor(int index, struct led_PackedColor *const color) {
    if(index > LED_CHAIN_LENGTH || !color) {
        return;
    }
    
    LedState[index].color = *color;
}
