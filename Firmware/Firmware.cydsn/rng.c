#include "rng.h"
#include <project.h>

void rng_Start(uint8_t seed) {
    BatterySenseADC_Start();
    BatterySenseADC_StartConvert();
    
    PRS_1_Start();
    
    //int16_t v = BatterySenseADC_GetResult16(0);
    //PRS_1_ResetSeedInit(seed | v);
    PRS_1_ResetSeedInit(seed);
}

uint8_t rng_GetByte(void) {
    return PRS_1_Read();
}

bool rng_IsCoinHeads(void) {
    return rng_GetByte() > 127;
}
