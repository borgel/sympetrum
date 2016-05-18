#ifndef RNG_H__
#define RNG_H__
    
#include <stdint.h>
    
void rng_Start(uint8_t seed);
uint8_t rng_GetByte(void);
    
#endif//RNG_H__