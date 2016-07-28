#include "ir.h"
#include "leds.h"
#include "debprint.h"
#include "systime.h"

#include <project.h>
#include <stdbool.h>


#define IR_HEADER           0x7FFF
#define IR_FOOTER           0x0000
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
    struct ir_Message msgInProgress;
    
    
    struct ir_Message const *msgToIgnore;
};
static const struct ir_state stateNULL = {
    .currentBit = 0,
    .incomingBuffer = 0,
    .msgState = IRR_IDLE,
    .msgInProgress.body = 0,
    .msgToIgnore = 0,
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
            state.msgInProgress.body = 0;
            state.currentBit = 0;
            state.incomingBuffer = 0;
        }
    }
    
    if(state.msgState == IRR_MSG_IN_PROGRESS) {
        state.msgInProgress.body <<= 1;
        state.msgInProgress.body |= (Pin_rx_decoded_data_Read() & 0x1);
        
        state.currentBit++;
        
        if(state.currentBit > IR_MSG_LENGTH_BITS) {
            state.msgState = IRR_MSG_COMPLETE;
        }
    }
    
    IRDataIncomingInter_ClearPending();
}

void ir_Start() {
    state = stateNULL;
    
    IR_Transceiver_Start();
    PWM_1_Start();
    IRLedControl_Write(0);
    IRDataIncomingInter_StartEx(IncomingIRISR);
}

/*
When receiving data, if this message comes in ignore it. Useful for
ignoring your own reflected glory.
*/
void ir_SetBeaconToIgnore(struct ir_Message const *msg) {
    state.msgToIgnore = msg;
}

void ir_Send(struct ir_Message const *msg) {
    uint16_t txdata[] = {
        IR_HEADER,
        msg->body,
    };
    
    //enable the LEDs for transmit
    IRLedControl_Write(1);
    
    // length in (16 bit) words
    IR_Transceiver_PutArray(txdata, 2);
    
    IRLedControl_Write(0);
}

void ir_GiveTime(void){
    if(state.msgState == IRR_MSG_COMPLETE) {
        if(state.msgToIgnore && state.msgToIgnore->body == state.msgInProgress.body) {
            debprint("Ignoring msg at %dMS\r\n", systime_GetTimeMS());
        }
        else {
            debprint("Got an IR beacon: 0x%04X @ %dMS\r\n", state.msgInProgress.body, systime_GetTimeMS());
            
            //TODO react
            //TODO enqueue message into incoming msg buffer
            
            UserLED_Write(!UserLED_Read());
        }
        state.msgState = IRR_IDLE;
    }
}
