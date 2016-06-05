#include "ir.h"
#include "leds.h"
#include "debprint.h"

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


static bool StartDataRX;
CY_ISR(IncomingIRISR) {
    //debprint("Incoming ISR? Starting SPI RX\n");
    
    StartDataRX = true;
}


static bool ir_ValidateMessage(union ir_RawMessage *msg);

void ir_Start(void) {
    IR_Transceiver_Start();
    PWM_1_Start();
    IRLedControl_Write(0);
    StartDataRX = false;
    IRDataIncomingInter_StartEx(IncomingIRISR);
}

void ir_Send(struct ir_Message *msg) {
    union ir_RawMessage m = {.msg = *msg, .toggle = 1};
    
    IRLedControl_Write(1);
    IR_Transceiver_WriteTxData(m.raw);
    //IR_Transceiver_WriteTxData(0xF0F);
    IRLedControl_Write(0);
}

static void ir_PrintMessage(struct ir_Message *const m) {
    debprint("Addr: 0x%x - Data 0x%x\n", m->address, m->command);
}

bool ir_Receive(struct ir_Message *msg) {
    union ir_RawMessage m;
    
    m.raw = IR_Transceiver_ReadRxData();
    
    *msg = m.msg;
    return true;
    
    //FIXME re-enable
    /*
    if(ir_ValidateMessage(&m)) {
        *msg = m.msg;
        return true;
    }
    return false;
    */
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

void ir_GiveTime(void) {
    if(StartDataRX) {
        debprint("Starting rx...\n");
        
        struct ir_Message m;
        ir_Receive(&m);
        
        debprint("Message received! Contents are\n");
        ir_PrintMessage(&m);
        
        //TODO react
        
        led_DisplayPattern();
        
        StartDataRX = false;
    }
}
