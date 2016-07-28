#ifndef COLOR_H__
#define COLOR_H__
    
#include <stdint.h>
    
struct color_Color {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
};
    
void color_GetRandomColor(struct color_Color* c);
void color_HSV2RGB(struct color_ColorHSV const *hsv, struct color_ColorRGB *rgb);
    
#endif//COLOR_H__
