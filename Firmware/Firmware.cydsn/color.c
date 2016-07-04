/*
*/

#include "color.h"
#include "rng.h"

#include <project.h>

void color_GetRandomColor(struct color_PackedColor* c) {
    c->r = rng_GetByte();
    c->g = rng_GetByte();
    c->b = rng_GetByte();
}
