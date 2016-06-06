#ifndef IR_H__
#define IR_H__
    
#include <stdint.h>
#include <stdbool.h>
    
struct ir_Message {
    uint8_t address         :5;
    uint8_t command         :6;
}__attribute__((packed));
    
void ir_Start(void);

void ir_Send(struct ir_Message *msg);
//bool ir_Receive(struct ir_Message *msg);

void ir_GiveTime(void);

#endif//IR_H__
