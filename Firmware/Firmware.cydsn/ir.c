#include "ir.h"
#include "leds.h"
#include "debprint.h"

#include <project.h>
#include <stdbool.h>

struct ir_state {
    bool seenFirstBit;
    bool MessageReceived;
    
    int currentBit;
    uint16_t msgInProgress;
};
static const struct ir_state stateNULL = {};
static struct ir_state state;

//the header always used for a message, 0b11
#define RC5_HEADER              0x3

//this represents a Phillips RC-5 data packet
union ir_RawMessage {
    uint16_t raw;
    struct {
        uint8_t header          :2;
        uint8_t toggle          :1;
        struct ir_Message msg;
        uint8_t footer          :2;
    }__attribute__((packed));
}__attribute__((packed));


static bool MessageReceived;
CY_ISR(IncomingIRISR) {
    //there is always one trash bit when the system comes up. Ignore it
    if(state.seenFirstBit) {
        //sample data in pin and shift into msg
        //when msg full, set 'msg arrived' flag
        
        //Pin_rx_decoded_data_Read
        state.msgInProgress |= Pin_rx_decoded_data_Read();
        state.msgInProgress <<= 1;
        
        state.currentBit++;
        //TODO macro this
        if(state.currentBit == 16) {
            state.MessageReceived = true;
        }
    }
    else {
        state.seenFirstBit = true;
    }
    
    IRDataIncomingInter_ClearPending();
}


static bool ir_ValidateMessage(union ir_RawMessage *msg);

void ir_Start(void) {
    state = stateNULL;
    
    IR_Transceiver_Start();
    PWM_1_Start();
    IRLedControl_Write(0);
    MessageReceived = false;
    IRDataIncomingInter_StartEx(IncomingIRISR);
}

void ir_Send(struct ir_Message *msg) {
    union ir_RawMessage m = {
        .header = RC5_HEADER,
        .toggle = 1,
        .msg = *msg, 
        .footer = RC5_HEADER,
    };
    
    IRLedControl_Write(1);
    //FIXME needed?
    //m.raw = __builtin_bswap16(m.raw);
    //IR_Transceiver_WriteTxData(m.raw);
    IR_Transceiver_WriteTxData(0x55AA);
    IRLedControl_Write(0);
}

static void ir_PrintMessage(struct ir_Message *const m) {
    debprint("Addr: 0x%x - Data 0x%x\r\n", m->address, m->command);
}

/*
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
*/

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

void ir_GiveTime(void){
    if(state.MessageReceived) {
        debprint("16 bits have been clocked in...\r\n");
        debprint("Raw as u16 = 0x%04X\r\n", state.msgInProgress);
        
        state.currentBit = 0;
        state.msgInProgress = 0;
        
        //debprint("%d\r\n", Pin_rx_decoded_data_Read());
        
        /*
        struct ir_Message m;
        ir_Receive(&m);
        
        debprint("Message received! Contents are...\r\n\t");
        ir_PrintMessage(&m);
        
        
        //TODO react
        
        led_DisplayPattern();
        */
        
        state.MessageReceived = false;
    }
}
