/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

struct led_header {
    uint8_t header          :3;
    uint8_t global          :5;
    
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__((packed));

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    SPIM_1_Start();
    
    //0x7, 0x1F is max
    struct led_header head = {0x7, 0x1F/5, 0, 5, 0};

    int i;
    for(;;)
    {
        //start frame
        SPIM_1_WriteByte(0x00);
        SPIM_1_WriteByte(0x00);
        SPIM_1_WriteByte(0x00);
        SPIM_1_WriteByte(0x00);
        
        
        for(i = 0; i < 6; i++) {
            //all 0s for all off
            SPIM_1_PutArray((uint8_t*)&head, sizeof(head));
        }
        
        //end frame
        SPIM_1_WriteByte(0xFF);
        SPIM_1_WriteByte(0xFF);
        SPIM_1_WriteByte(0xFF);
        SPIM_1_WriteByte(0xFF);
        
    }
}

/* [] END OF FILE */
