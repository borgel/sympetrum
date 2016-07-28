#ifndef COLOR_H__
#define COLOR_H__
    
#include <stdint.h>
    
struct color_ColorRGB {
    uint8_t     b;
    uint8_t     g;
    uint8_t     r;
};

struct color_ColorHSV {
    uint8_t     h;
    uint8_t     s;
    uint8_t     v;
};
    
void color_GetRandomColor(struct color_ColorRGB* c);
void color_HSV2RGB(struct color_ColorHSV const *hsv, struct color_ColorRGB *rgb);
    
#endif//COLOR_H__
