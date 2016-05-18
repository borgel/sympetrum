#ifndef LED_H__
#define LED_H__
    
#include <stdint.h>
    
struct led_PackedColor {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
}__attribute__((packed));
    
void led_Start(void);
void led_SetColor(int index, struct led_PackedColor *const color);
void led_GiveTime(void);
    
#endif//LED_H__
