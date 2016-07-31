#ifndef BOARD_ID_H__
#define BOARD_ID_H__
    
#include <stdint.h>
    
//7 bits
#define BOARD_ID_MAX            127
    
uint8_t bid_GetID(void);
    
#endif//BOARD_ID_H__
