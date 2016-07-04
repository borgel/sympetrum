#ifndef RNG_H__
#define RNG_H__
    
#include <stdint.h>
#include <stdbool.h>
    
void rng_Start(uint8_t seed);
uint8_t rng_GetByte(void);
bool rng_IsCoinHeads(void);
    
#endif//RNG_H__