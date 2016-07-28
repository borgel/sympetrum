#ifndef IR_H__
#define IR_H__
    
#include <stdint.h>
#include <stdbool.h>
    
struct ir_Message {
    uint16_t body;
};
    
void ir_Start(void);

void ir_Send(struct ir_Message const *msg);
//bool ir_Receive(struct ir_Message *msg);
void ir_SetBeaconToIgnore(struct ir_Message const *msg);

void ir_GiveTime(void);

#endif//IR_H__
