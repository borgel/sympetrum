#ifndef IR_H__
#define IR_H__
    
#include <stdint.h>
#include <stdbool.h>
    
//referring to data stored in the IR messages
#define IR_MESSAGE_DATA_ID_OFFSET      0
    
struct ir_Message {
    union {
        uint16_t body;
        uint8_t data[2];
    };
};
    
void ir_Start(void);

void ir_Send(struct ir_Message const *msg);
//bool ir_Receive(struct ir_Message *msg);
void ir_SetBeaconToIgnore(struct ir_Message const *msg);

void ir_GiveTime(void);

#endif//IR_H__
