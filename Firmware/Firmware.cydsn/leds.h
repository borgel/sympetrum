#ifndef LED_H__
#define LED_H__
    
#include <stdint.h>
#include "color.h"

#define                 LED_CHAIN_LENGTH        10
    
void led_Start(void);
void led_SetColor(int index, struct color_ColorRGB *const color);
void led_DisplayPattern(void);
    
#endif//LED_H__
