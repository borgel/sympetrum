#include "ir.h"

#include <project.h>
#include <stdbool.h>

//the header always used for a message, 0b11
#define RC5_HEADER              0x3

//this represents a Phillips RC-5 data packet
union ir_RawMessage {
    uint16_t raw;
    struct {
        uint8_t header          :2;
        uint8_t toggle          :1;
        struct ir_Message msg;
    }__attribute__((packed));
}__attribute__((packed));

static bool ir_ValidateMessage(union ir_RawMessage *msg);

void ir_Start(void) {
    IR_Transceiver_Start();
    PWM_1_Start();
}

void ir_Send(struct ir_Message *msg) {
    union ir_RawMessage m = {.msg = *msg, .toggle = 1};
    
    //TODO explicitely enable/disable LED outputs around this
    IR_Transceiver_WriteTxData(m.raw);
    //IR_Transceiver_WriteTxData(0xF0F);
}

bool ir_Receive(struct ir_Message *msg) {
    union ir_RawMessage m;
    
    m.raw = IR_Transceiver_ReadRxData();
    
    if(ir_ValidateMessage(&m)) {
        *msg = m.msg;
        return true;
    }
    return false;
}

static bool ir_ValidateMessage(union ir_RawMessage *msg) {
    if(!msg) {
        return false;
    }
    
    //TODO use toggle bit as parity bit?
    
    if(msg->header != RC5_HEADER) {
        return false;
    }
    return true;
}
