/*
*/

#include "color.h"
#include "rng.h"

#include <project.h>

/*
Get a random color weighted towards an extreme in one channel.
*/
void color_GetRandomColor(struct color_ColorRGB* c) {
    switch(rng_GetByte() % 3) {
    case 0:
        c->r = rng_GetByte();
        c->g /= 3;
        c->b /= 3;
        break;
        
    case 1:
        c->r /= 3;
        c->g = rng_GetByte();
        c->b /= 3;
        break;
    
    case 2:
        c->r /= 3;
        c->g /= 3;
        c->b = rng_GetByte();
        break;
    }
}

/*
Algorithm adapted from http://stackoverflow.com/a/6930407.
*/
void color_HSV2RGB(struct color_ColorHSV const *hsv, struct color_ColorRGB *rgb) {
    float      hh, p, q, t, ff;
    long        i;

    if(hsv->s <= 0) {       // < is bogus, just shuts up warnings
        rgb->r = hsv->v;
        rgb->g = hsv->v;
        rgb->b = hsv->v;
        return;
    }
    hh = hsv->h;
    if(hh >= 360.0) {
        hh = 0.0;
    }
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = hsv->v * (1.0 - hsv->s);
    q = hsv->v * (1.0 - (hsv->s * ff));
    t = hsv->v * (1.0 - (hsv->s * (1.0 - ff)));

    switch(i) {
        case 0:
            rgb->r = hsv->v;
            rgb->g = t;
            rgb->b = p;
            break;
        case 1:
            rgb->r = q;
            rgb->g = hsv->v;
            rgb->b = p;
            break;
        case 2:
            rgb->r = p;
            rgb->g = hsv->v;
            rgb->b = t;
            break;

        case 3:
            rgb->r = p;
            rgb->g = q;
            rgb->b = hsv->v;
            break;
        case 4:
            rgb->r = t;
            rgb->g = p;
            rgb->b = hsv->v;
            break;
        case 5:
        default:
            rgb->r = hsv->v;
            rgb->g = p;
            rgb->b = q;
            break;
        }
}
