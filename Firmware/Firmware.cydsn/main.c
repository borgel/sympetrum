/*
*/
#include <project.h>

#include <stdint.h>


int main()
{
    CyGlobalIntEnable;
    
    IR_Transmit_Start();
    
    uint8_t count = 0;
    
    uint16_t data;
    for(;;) {
        data = count;
        
        IR_Transmit_WriteTxData(data);
        
        count++;
        CyDelay(100);
    }
}

/* [] END OF FILE */
