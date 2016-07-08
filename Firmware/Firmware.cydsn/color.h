#ifndef COLOR_H__
#define COLOR_H__
    
#include <stdint.h>
    
struct color_PackedColor {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
}__attribute__((packed));
    
void color_GetRandomColor(struct color_PackedColor* c);
    
#endif//COLOR_H__
