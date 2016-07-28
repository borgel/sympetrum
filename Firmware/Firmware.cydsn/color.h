#ifndef COLOR_H__
#define COLOR_H__
    
#include <stdint.h>
    
struct color_Color {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
};
    
void color_GetRandomColor(struct color_Color* c);
    
#endif//COLOR_H__
