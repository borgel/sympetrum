#include "ir.h"
#include "leds.h"
#include "debprint.h"

#include <project.h>
#include <stdbool.h>


#define IR_HEADER           0x7FFF
#define IR_FOOTER           0x0000

union ir_RawMessage {
    uint16_t raw;
    uint8_t array[2];
};

//#define IR_MSG_LENGTH_BITS      (8 * sizeof(union ir_RawMessage))
#define IR_MSG_LENGTH_BITS          16

enum IR_RECEIVE_STATE {
    IRR_IDLE,
    IRR_MSG_IN_PROGRESS,
    IRR_MSG_COMPLETE,
};

struct ir_state {
    int currentBit;
    uint16_t incomingBuffer;
    enum IR_RECEIVE_STATE msgState;
    union ir_RawMessage msgInProgress;
};
static const struct ir_state stateNULL = {
    .currentBit = 0,
    .incomingBuffer = 0,
    .msgState = IRR_IDLE,
    .msgInProgress.raw = 0,
};
static struct ir_state state;

//triggered on rising data clock edge. Sample incoming data to RX the bit
CY_ISR(IncomingIRISR) {
    if(state.msgState == IRR_IDLE) {
        state.incomingBuffer <<= 1;
        state.incomingBuffer |= (Pin_rx_decoded_data_Read() & 0x1);
        
        //check if we have a start sequence
        if(state.incomingBuffer == IR_HEADER) {
            state.msgState = IRR_MSG_IN_PROGRESS;
            state.msgInProgress.raw = 0;
            state.currentBit = 0;
            
            //FIXME needed?
            state.incomingBuffer = 0;
        }
    }
    
    if(state.msgState == IRR_MSG_IN_PROGRESS) {
        state.msgInProgress.raw <<= 1;
        state.msgInProgress.raw |= (Pin_rx_decoded_data_Read() & 0x1);
        
        state.currentBit++;
        
        if(state.currentBit > IR_MSG_LENGTH_BITS) {
            state.msgState = IRR_MSG_COMPLETE;
        }
    }
    
    IRDataIncomingInter_ClearPending();
}

void ir_Start(void) {
    state = stateNULL;
    
    IR_Transceiver_Start();
    PWM_1_Start();
    IRLedControl_Write(0);
    IRDataIncomingInter_StartEx(IncomingIRISR);
}

void ir_Send(struct ir_Message const *msg) {
    uint16_t txdata[] = {
        IR_HEADER,
        msg->body,
    };
    
    (void)*msg;
    
    IRLedControl_Write(1);
    //CyDelay(5);
    
    //IR_Transceiver_WriteTxData(IR_HEADER);
    //IR_Transceiver_WriteTxData(m.raw);
    //IR_Transceiver_WriteTxData(IR_FOOTER);
    
    //header implies len is in bytes? seems to be wrong (send trash)
    IR_Transceiver_PutArray(txdata, 2);
    
    //wait until send complete?
    //CyDelay(5);
    IRLedControl_Write(0);
}

/*
static void ir_PrintMessage(struct ir_Message *const m) {
    debprint("Message Body: 0x%x\r\n", m->body);
}

bool ir_Receive(struct ir_Message *msg) {
    union ir_RawMessage m;
    
    m.raw = IR_Transceiver_ReadRxData();
    //m.raw = __builtin_bswap16(m.raw);
    
    debprint("Raw as u16 = 0x%04X\r\n", m.raw);
    
    *msg = m.msg;
    return true;
    
    //FIXME re-enable
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
    
    //TODO send message twice and make sure both copies match? parity bit?    
    return true;
}
*/

void ir_GiveTime(void){
    if(state.msgState == IRR_MSG_COMPLETE) {
        //debprint("16 bits have been clocked in...\r\n");
        debprint("0x%04X\r\n", state.msgInProgress.raw);
        
        
        //TODO react
        //TODO enqueue message into incoming msg buffer
        
        
        UserLED_Write(!UserLED_Read());
        
        state.msgState = IRR_IDLE;
    }
}
