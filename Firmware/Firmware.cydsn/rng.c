#include "rng.h"
#include <project.h>

void rng_Start(uint8_t seed) {
    PRS_1_Start();
    PRS_1_ResetSeedInit(seed);
}

uint8_t rng_GetByte(void) {
    return PRS_1_Read();
}

bool rng_IsCoinHeads(void) {
    return rng_GetByte() > 127;
}
