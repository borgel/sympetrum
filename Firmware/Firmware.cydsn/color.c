/*
*/

#include "color.h"
#include "rng.h"

#include <project.h>

/*
Get a random color weighted towards an extreme in one channel.
*/
void color_GetRandomColor(struct color_Color* c) {
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
